#include "charcoaldbhelpers.h"

#include "common/logs.h"
#include "common/tags.h"
#include "database/dbhelpers.h"

#include <QMetaEnum>
#include <QString>

#include <QJsonDocument>
#include <QJsonObject>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

// Const static
const QHash<Enums::SupplyChainAction, QString> CharcoalDbHelpers::m_supplyActionMap = {
    { Enums::SupplyChainAction::LoggingBeginning, QStringLiteral("LB") },
    { Enums::SupplyChainAction::LoggingEnding, QStringLiteral("LE") },
    { Enums::SupplyChainAction::CarbonizationBeginning, QStringLiteral("CB") },
    { Enums::SupplyChainAction::CarbonizationEnding, QStringLiteral("CE") },
    { Enums::SupplyChainAction::LoadingAndTransport, QStringLiteral("TR") },
    { Enums::SupplyChainAction::LocalReception, QStringLiteral("LR") },
    { Enums::SupplyChainAction::Reception, QStringLiteral("RE") }
};

const QHash<Enums::PackageType, QString> CharcoalDbHelpers::m_packageTypeMap = {
    { Enums::PackageType::Plot, QStringLiteral("Plot") },
    { Enums::PackageType::Harvest, QStringLiteral("Harvest") },
    { Enums::PackageType::Transport, QStringLiteral("Transport") }
};

// Static
QHash<int, Enums::SupplyChainAction> CharcoalDbHelpers::m_supplyActionDbMap;
QHash<int, Enums::PackageType> CharcoalDbHelpers::m_packageTypeDbMap;
QHash<int, Enums::OvenType> CharcoalDbHelpers::m_ovenTypeDbMap;

qreal CharcoalDbHelpers::ovenVolume(const qreal width, const qreal length,
                                    const qreal height1, const qreal height2)
{
    if (height2 < 0) {
        return width * height1 * length;
    } else {
        const qreal small = std::min(height1, height2);
        const qreal big = std::max(height1, height2);
        const qreal triangleHeight = big - small;

        return ((small * length) + ((triangleHeight * length) / 2)) * width;
    }
}

QString CharcoalDbHelpers::propertiesToString(const QVariantMap &properties)
{
    return QJsonDocument::fromVariant(properties).toJson(QJsonDocument::Compact);
}

QString CharcoalDbHelpers::getPlotName(const QString &packageName)
{
    const QStringList parts(packageName.split(sep));

    if (parts.length() < 3) {
        qWarning() << RED("Invalid ID passed to getPlotId") << packageName;
        return QString();
    }

    const QStringList plot(parts.mid(0, 3));
    return plot.join(sep);
}

QString CharcoalDbHelpers::getHarvestName(const QString &packageName)
{
    const QStringList parts(packageName.split(sep));

    if (parts.length() < 4) {
        qWarning() << RED("Invalid ID passed to getHarvestId") << packageName;
        return QString();
    }

    const QStringList plot(parts.mid(0, 4));
    return plot.join(sep);
}

QString CharcoalDbHelpers::actionAbbreviation(const Enums::SupplyChainAction action)
{
    return m_supplyActionMap.value(action);
}

Enums::SupplyChainAction CharcoalDbHelpers::actionById(const QString &connectionName,
                                                       const int id)
{
    const Enums::SupplyChainAction cached = m_supplyActionDbMap.value(
        id, Enums::SupplyChainAction::Unknown);

    if (cached == Enums::SupplyChainAction::Unknown) {
        return cacheSupplyAction(connectionName, id);
    }

    return cached;
}

Enums::SupplyChainAction CharcoalDbHelpers::actionByName(const QString &actionName)
{
    return m_supplyActionMap.key(actionName, Enums::SupplyChainAction::Unknown);
}

/*!
 * Returns a list of all QR codes already registered (in various reception
 * events) for given \a transportId.
 */
QVariantList CharcoalDbHelpers::bagsInReceptions(const QString &connectionName, const int transportId)
{
    const int transportTypeId = CharcoalDbHelpers::getEventTypeId(
        connectionName, Enums::SupplyChainAction::LoadingAndTransport);

    QSqlQuery query(QString(), db::Helpers::databaseConnection(connectionName));

    query.prepare("SELECT properties FROM Events "
                  "WHERE entityId=:transportEntityId "
                  "AND typeId!=:transportTypeId");
    query.bindValue(":transportEntityId", transportId);
    query.bindValue(":transportTypeId", transportTypeId);

    QVariantList result;

    if (query.exec()) {
        while (query.next()) {
            const QByteArray propertiesString(query.value(Tags::properties).toByteArray());
            const QJsonDocument propertiersJson(QJsonDocument::fromJson(propertiesString));
            const QVariantMap properties(propertiersJson.toVariant().toMap());
            result.append(properties.value(Tags::webQrCodes).toList());
        }
    }

    return result;
}

int CharcoalDbHelpers::bagCountInTransport(const QString &connectionName, const int id)
{
    const int transportTypeId = CharcoalDbHelpers::getEventTypeId(
        connectionName, Enums::SupplyChainAction::LoadingAndTransport);

    QSqlQuery query(QString(), db::Helpers::databaseConnection(connectionName));

    query.prepare("SELECT properties FROM Events "
                  "WHERE entityId=:transportEntityId "
                  "AND typeId=:transportTypeId");
    query.bindValue(":transportEntityId", id);
    query.bindValue(":transportTypeId", transportTypeId);

    if (query.exec()) {
        query.next();
        const QByteArray propertiesString(query.value(Tags::properties).toByteArray());
        const QJsonDocument propertiersJson(QJsonDocument::fromJson(propertiesString));
        const QVariantMap properties(propertiersJson.toVariant().toMap());
        return properties.value(Tags::webQrCodes).toList().size();
    }

    qWarning() << RED("Getting bag count has failed!")
               << query.lastError().text()
               << "for query:" << query.lastQuery()
               << "DB:" << connectionName;

    return -1;
}

OvenDimensions CharcoalDbHelpers::defaultOvenDimensions(const QString &connectionName,
                                                        const int ovenId)
{
    QSqlQuery query(QString(), db::Helpers::databaseConnection(connectionName));
    OvenDimensions dimensions;
    query.prepare("SELECT oven_height, oven_height2, oven_width, oven_length, "
                  "type "
                  "FROM OvenTypes WHERE id=:ovenId");
    query.bindValue(":ovenId", ovenId);

    if (query.exec() == false) {
        qWarning() << RED("Getting metallic oven dimensions has failed!")
                   << query.lastError().text() << "for query:" << query.lastQuery();
        return dimensions;
    }

    query.next();

    if (query.value(Tags::type).toInt() == CharcoalDbHelpers::metalOvenType) {
        dimensions.height1 = query.value(Tags::webOvenHeight).toReal();
        dimensions.height2 = query.value(Tags::webOvenHeight2).isNull()?
            -1 : query.value(Tags::webOvenHeight2).toReal();

        if (qFuzzyIsNull(dimensions.height2)) {
            dimensions.height2 = -1;
        }

        dimensions.width = query.value(Tags::webOvenWidth).toReal();
        dimensions.length = query.value(Tags::webOvenLength).toReal();
        dimensions.hasFixedDimensions = true;

        return dimensions;
    } else {
        dimensions.hasFixedDimensions = false;
        return dimensions;
    }
}

QJsonObject CharcoalDbHelpers::dbPropertiesToJson(const QString &properties)
{
    const QJsonDocument propertiesDoc(QJsonDocument::fromJson(properties.toUtf8()));
    return propertiesDoc.object();
}

int CharcoalDbHelpers::getWebPackageId(const QString &connectionName, const int entityId)
{
    return getSimpleInteger(connectionName, "Entities", Tags::id, entityId, Tags::webId);
}

/*!
 * Returns all webIds for packages where name of the plot is \a plotName
 * or has \a parentId, using SQL connection \a connectionName.
 */
QVector<int> CharcoalDbHelpers::getWebPackageIds(const QString &connectionName,
                                                 const int transportId,
                                                 const int parentId)
{
    QSqlQuery query(QString(), db::Helpers::databaseConnection(connectionName));
    query.prepare("SELECT webId FROM Entities WHERE id=:transportId "
                  "OR parent=:parentId");
    query.bindValue(":transportId", transportId);
    query.bindValue(":parentId", parentId);

    if (query.exec() == false) {
        qWarning() << RED("Getting list of web IDs has failed!")
                   << query.lastError().text()
                   << "for query:" << query.lastQuery()
                   << "with params:" << transportId << parentId;
        return {};
    }

    QVector<int> result;
    bool ok = false;
    while (query.next()) {
        const int id = query.value(Tags::webId).toInt(&ok);
        if (ok) {
            result.append(id);
        }
    }

    return result;
}

int CharcoalDbHelpers::getVillageId(const QString &connectionName, const QString &name)
{
    return getSimpleInteger(connectionName, "Villages", Tags::name, name, Tags::id);
}

int CharcoalDbHelpers::getDestinationId(const QString &connectionName, const QString &name)
{
    return getSimpleInteger(connectionName, "Destinations", Tags::name, name, Tags::id);
}

int CharcoalDbHelpers::getParcelId(const QString &connectionName, const QString &parcel)
{
    return getSimpleInteger(connectionName, "Parcels", Tags::code, parcel, Tags::id);
}

int CharcoalDbHelpers::getTreeSpeciesId(const QString &connectionName, const QString &species)
{
    return  getSimpleInteger(connectionName, "TreeSpecies", Tags::name, species, Tags::id);
}

int CharcoalDbHelpers::getOvenId(const QString &connectionName, const int plotId,
                                 const QString &ovenName, const bool verbose)
{
    return getInteger(connectionName, "Ovens", { Tags::plot, Tags::name },
                      { plotId, ovenName }, Tags::id, QString(), verbose);
}

int CharcoalDbHelpers::getOvenTypeId(const QString &connectionName, const QString &ovenType)
{
    return getSimpleInteger(connectionName, "OvenTypes", Tags::type, ovenType, Tags::id,
                            db::QueryFlag::Verbose);
}

int CharcoalDbHelpers::getOvenTypeIdFromName(const QString &connectionName,
                                             const QString &name)
{
    return getSimpleInteger(connectionName, "OvenTypes", Tags::name, name, Tags::id,
                            db::QueryFlag::Verbose);
}

int CharcoalDbHelpers::getEntityIdFromWebId(const QString &connectionName,
                                            const int webId,
                                            const db::QueryFlags settings)
{
    return getSimpleInteger(connectionName, "Entities", Tags::webId, webId,
                            Tags::id, settings);
}

int CharcoalDbHelpers::getEntityTypeId(const QString &connectionName,
                                       const Enums::PackageType type)
{
    const int cached = m_packageTypeDbMap.key(type, -1);

    if (cached == -1) {
        if (cachePackageType(connectionName, type) == Enums::PackageType::Unknown) {
            return -1;
        }

        return m_packageTypeDbMap.key(type, -1);
    }

    return cached;
}

int CharcoalDbHelpers::getParentEntityId(const QString &connectionName, const int id)
{
    return getSimpleInteger(connectionName, "Entities", Tags::id, id, Tags::parent);
}

int CharcoalDbHelpers::getEventTypeId(const QString &connectionName,
                                      const Enums::SupplyChainAction action)
{
    const int cached = m_supplyActionDbMap.key(action, -1);

    if (cached == -1) {
        if (cacheSupplyAction(connectionName, action) == Enums::SupplyChainAction::Unknown) {
            return -1;
        }

        return m_supplyActionDbMap.key(action, -1);
    }

    return cached;
}

Enums::SupplyChainAction CharcoalDbHelpers::getEventTypeFromEventId(
    const QString &connectionName, const int eventId)
{
    const int typeId = getSimpleInteger(connectionName, "Events",
                                        Tags::id, eventId,
                                        Tags::typeId,
                                        db::QueryFlag::Verbose);

    return actionById(connectionName, typeId);
}

int CharcoalDbHelpers::getEventIdFromWebId(const QString &connectionName,
                                           const int webId, const db::QueryFlags settings)
{
    return getSimpleInteger(connectionName, "Events", Tags::webId, webId, Tags::id, settings);
}

int CharcoalDbHelpers::getEventId(const QString &connectionName,
                                  const int entityId,
                                  const int eventTypeId,
                                  qint64 timestamp,
                                  const bool verbose)
{
    return getInteger(connectionName, "Events",
                      { Tags::entityId, Tags::typeId, Tags::date },
                      { entityId, eventTypeId, timestamp },
                      Tags::id, "AND properties IS NOT NULL", verbose);
}

int CharcoalDbHelpers::getEventId(const QString &connectionName,
                                  qint64 timestamp,
                                  const db::QueryFlags settings)
{
    return getSimpleInteger(connectionName, "Events", Tags::date, timestamp, Tags::id,
                            settings);
}

ContinueEvent CharcoalDbHelpers::getContinueEvent(const QString &connectionName,
                                                  const int eventTypeId)
{
    QSqlQuery q(QString(), db::Helpers::databaseConnection(connectionName));
    q.prepare("SELECT id, entityId "
              "FROM Events "
              "WHERE isPaused=1 AND typeId=:eventTypeId");
    q.bindValue(":eventTypeId", eventTypeId);

    if (q.exec() && q.next()) {
        ContinueEvent event;
        event.entityId = q.value(Tags::entityId).toInt();
        event.eventId = q.value(Tags::id).toInt();
        return event;
    }

    return ContinueEvent();
}

int CharcoalDbHelpers::getEventTypeId(const QString &connectionName, const QString &action)
{
    const Enums::SupplyChainAction chainAction = m_supplyActionMap.key(action);
    return getEventTypeId(connectionName, chainAction);
}

int CharcoalDbHelpers::getSimpleInteger(const QString &connectionName,
                                        const QString &table,
                                        const QString &matchColumn,
                                        const QVariant &matchValue,
                                        const QString &returnColumn,
                                        const db::QueryFlags settings)
{
    QString ordering("DESC");
    if (settings.testFlag(db::QueryFlag::MatchFirst)) {
        ordering = "ASC";
    }

    int result = -1;
    const QString queryString(
        QString("SELECT %1 FROM %2 WHERE %3=:value ORDER BY %1 %4 LIMIT 1")
            .arg(returnColumn, table, matchColumn, ordering));

    QSqlQuery query(QString(), db::Helpers::databaseConnection(connectionName));
    query.prepare(queryString);
    query.bindValue(":value", matchValue);

    if (query.exec() && query.next()) {
        result = query.value(returnColumn).toInt();
    } else if (settings.testFlag(db::QueryFlag::Verbose)) {
        qWarning() << RED("Unable to fetch")
                   << connectionName << table << matchColumn << matchValue
                   << returnColumn
                   << "SQL error:" << query.lastError().text()
                   << "For query:" << query.lastQuery();
    }

    return result;
}

int CharcoalDbHelpers::getInteger(const QString &connectionName,
                                  const QString &table,
                                  const QStringList &matchColumns,
                                  const QVariantList &matchValues,
                                  const QString &returnColumn,
                                  const QString &extra,
                                  const bool verbose)
{
    int result = -1;

    QString matchString;
    for (const QString &column : matchColumns) {
        if (matchString.isEmpty() == false) {
            matchString.append(" AND ");
        }

        matchString.append(column + "=:" + column);
    }

    const QString queryString(QString("SELECT %1 FROM %2 WHERE %3 %4").arg(
        returnColumn, table, matchString, extra));

    QSqlQuery query(QString(), db::Helpers::databaseConnection(connectionName));
    query.prepare(queryString);
    for (const QString &column : matchColumns) {
        query.bindValue(":" + column, matchValues.at(matchColumns.indexOf(column)));
    }

    if (query.exec() && query.next()) {
        result = query.value(returnColumn).toInt();
    } else if (verbose) {
        qWarning() << RED("Unable to fetch")
                   << connectionName << table << matchColumns << matchValues
                   << returnColumn
                   << "SQL error:" << query.lastError().text()
                   << "For query:" << query.lastQuery();
    }

    return result;
}

QString CharcoalDbHelpers::getParcelCode(const QString &connectionName, const int id)
{
    return getSimpleString(connectionName, "Parcels", Tags::id, id, Tags::code,
                           db::QueryFlag::Verbose);
}

QString CharcoalDbHelpers::getVillageName(const QString &connectionName, const int id)
{
    return getSimpleString(connectionName, "Villages", Tags::id, id, Tags::name,
                           db::QueryFlag::Verbose);
}

QString CharcoalDbHelpers::getTreeSpeciesName(const QString &connectionName, const int id)
{
    return getSimpleString(connectionName, "TreeSpecies", Tags::id, id, Tags::name,
                           db::QueryFlag::Verbose);
}

QString CharcoalDbHelpers::getDestinationName(const QString &connectionName, const int id)
{
    return getSimpleString(connectionName, "Destinations", Tags::id, id, Tags::name,
                           db::QueryFlag::Verbose);
}

QString CharcoalDbHelpers::getOvenLetter(const QString &connectionName, const int ovenId)
{
    return getSimpleString(connectionName, "Ovens", Tags::id, ovenId, Tags::name);
}

QString CharcoalDbHelpers::getEventType(const QString &connectionName, const int typeId)
{
    const Enums::SupplyChainAction action = actionById(connectionName, typeId);
    return m_supplyActionMap.value(action);
}

QString CharcoalDbHelpers::getEntityName(const QString &connectionName, const int entityId)
{
    return getSimpleString(connectionName, "Entities", Tags::id, entityId, Tags::name);
}

Enums::PackageType CharcoalDbHelpers::getEntityType(const QString &connectionName,
                                                    const int typeId)
{
    const Enums::PackageType cached = m_packageTypeDbMap.value(
        typeId, Enums::PackageType::Unknown);

    if (cached == Enums::PackageType::Unknown) {
        return cachePackageType(connectionName, typeId);
    }

    return cached;
}

QString CharcoalDbHelpers::getSimpleString(const QString &connectionName,
                                           const QString &table,
                                           const QString &matchColumn,
                                           const QVariant &matchValue,
                                           const QString &returnColumn,
                                           const db::QueryFlags settings)
{
    QString result;
    const QString queryString(QString("SELECT %1 FROM %2 WHERE %3=:value").arg(
        returnColumn, table, matchColumn));

    QSqlQuery query(QString(), db::Helpers::databaseConnection(connectionName));
    query.prepare(queryString);
    query.bindValue(":value", matchValue);

    if (query.exec() && query.next()) {
        result = query.value(returnColumn).toString();
    } else if (settings.testFlag(db::QueryFlag::Verbose)) {
        qWarning() << RED("Unable to fetch")
                   << connectionName << table << matchColumn << matchValue
                   << returnColumn
                   << "SQL error:" << query.lastError().text()
                   << "For query:" << query.lastQuery();
    }

    return result;
}

Enums::SupplyChainAction CharcoalDbHelpers::cacheSupplyAction(
    const QString &connectionName, const int actionId)
{
    const QString name(getSimpleString(connectionName, "EventTypes",
                                       Tags::id, actionId, Tags::actionName,
                                       db::QueryFlag::Verbose));
    const Enums::SupplyChainAction mapped = m_supplyActionMap.key(
        name, Enums::SupplyChainAction::Unknown);

    if (mapped == Enums::SupplyChainAction::Unknown) {
        qWarning() << RED("Invalid action ID!") << actionId << mapped;
        return Enums::SupplyChainAction::Unknown;
    }

    m_supplyActionDbMap.insert(actionId, mapped);

    return mapped;
}

Enums::SupplyChainAction CharcoalDbHelpers::cacheSupplyAction(
    const QString &connectionName, const Enums::SupplyChainAction action)
{
    const int actionId(getSimpleInteger(connectionName, "EventTypes",
                                        Tags::actionName, m_supplyActionMap.value(action),
                                        Tags::id, db::QueryFlag::Verbose));

    if (action == Enums::SupplyChainAction::Unknown || actionId == -1) {
        qWarning() << RED("Invalid type ID!") << actionId << action;
        return Enums::SupplyChainAction::Unknown;
    }

    m_supplyActionDbMap.insert(actionId, action);

    return action;
}

Enums::PackageType CharcoalDbHelpers::cachePackageType(const QString &connectionName,
                                                       const int typeId)
{
    const QString name(getSimpleString(connectionName, "EntityTypes",
                                       Tags::id, typeId, Tags::name,
                                       db::QueryFlag::Verbose));
    const Enums::PackageType mapped = m_packageTypeMap.key(
        name, Enums::PackageType::Unknown);

    if (mapped == Enums::PackageType::Unknown) {
        qWarning() << RED("Invalid type ID!") << typeId << mapped;
        return Enums::PackageType::Unknown;
    }

    m_packageTypeDbMap.insert(typeId, mapped);

    return mapped;
}

Enums::PackageType CharcoalDbHelpers::cachePackageType(const QString &connectionName,
                                                       const Enums::PackageType type)
{
    const int typeId(getSimpleInteger(connectionName, "EntityTypes",
                                      Tags::name, m_packageTypeMap.value(type),
                                      Tags::id, db::QueryFlag::Verbose));

    if (type == Enums::PackageType::Unknown || typeId == -1) {
        qWarning() << RED("Invalid type ID!") << typeId << type;
        return Enums::PackageType::Unknown;
    }

    m_packageTypeDbMap.insert(typeId, type);

    return type;
}

Enums::OvenType CharcoalDbHelpers::getOvenType(const QString &connectionName,
                                               const int typeId)
{
    const auto cachedType = m_ovenTypeDbMap.value(typeId, Enums::OvenType::Unknown);

    if (cachedType == Enums::OvenType::Unknown) {
        QSqlQuery q(QString(), db::Helpers::databaseConnection(connectionName));
        q.prepare("SELECT type FROM OvenTypes WHERE id=:typeId");
        q.bindValue(":typeId", typeId);
        if (q.exec() == false) {
            qWarning() << RED("Getting oven type has failed!")
                       << q.lastError().text() << "for query:" << q.lastQuery();
            return {};
        }

        q.next();

        const int type(q.value(Tags::type).toInt());
        const bool isMetallic = (type == CharcoalDbHelpers::metalOvenType);

        Enums::OvenType toInsert = Enums::OvenType::Traditional;
        if (isMetallic) {
            toInsert = Enums::OvenType::Metallic;
        }
        m_ovenTypeDbMap.insert(typeId, toInsert);

        return toInsert;
    } else {
        return cachedType;
    }

    return Enums::OvenType::Unknown;
}

int CharcoalDbHelpers::getEntityIdFromName(const QString &connectionName,
                                           const QString &name)
{
    return getSimpleInteger(connectionName, "Entities", Tags::name, name,
                            Tags::id, db::QueryFlag::Silent);
}
