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

int CharcoalDbHelpers::bagCountInTransport(const QString &connectionName, const int id)
{
    QSqlQuery query(QString(), db::Helpers::databaseConnection(connectionName));

    query.prepare("SELECT properties FROM Events WHERE entityId=:transportEntityId");
    query.bindValue(":transportEntityId", id);

    if (query.exec()) {
        query.next();
        const QByteArray propertiesString(query.value("properties").toByteArray());
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

QVariantList CharcoalDbHelpers::defaultOvenDimensions(const QString &connectionName,
                                                      const int ovenType)
{
    QSqlQuery query(QString(), db::Helpers::databaseConnection(connectionName));
    QVariantList dimensions;
    if (ovenType == CharcoalDbHelpers::metalOvenType) {
        query.prepare("SELECT oven_height, oven_width, oven_length "
                      "FROM OvenTypes WHERE type=:ovenType");
        query.bindValue(":ovenType", ovenType);

        if (query.exec() == false) {
            qWarning() << RED("Getting metallic oven dimensions has failed!")
                       << query.lastError().text() << "for query:" << query.lastQuery();
            return QVariantList{ 0, 0, 0 };
        }

        query.next();
        dimensions.append(query.value("oven_height").toDouble());
        dimensions.append(query.value("oven_width").toDouble());
        dimensions.append(query.value("oven_length").toDouble());
    } else {
        return QVariantList{ 0, 0, 0 };
    }

    return dimensions;
}

int CharcoalDbHelpers::getWebPackageId(const QString &connectionName, const int entityId)
{
    return getSimpleInteger(connectionName, "Entities", "id", entityId, "webId");
}

int CharcoalDbHelpers::getVillageId(const QString &connectionName, const QString &name)
{
    return getSimpleInteger(connectionName, "Villages", "name", name, "id");
}

int CharcoalDbHelpers::getDestinationId(const QString &connectionName, const QString &name)
{
    return getSimpleInteger(connectionName, "Destinations", "name", name, "id");
}

int CharcoalDbHelpers::getParcelId(const QString &connectionName, const QString &parcel)
{
    return getSimpleInteger(connectionName, "Parcels", "code", parcel, "id");
}

int CharcoalDbHelpers::getTreeSpeciesId(const QString &connectionName, const QString &species)
{
    return  getSimpleInteger(connectionName, "TreeSpecies", "name", species, "id");
}

int CharcoalDbHelpers::getOvenId(const QString &connectionName, const int plotId,
                                 const QString &ovenName, const bool verbose)
{
    return getInteger(connectionName, "Ovens", { "plot", "name" },
                      { plotId, ovenName }, "id", QString(), verbose);
}

int CharcoalDbHelpers::getOvenTypeId(const QString &connectionName, const QString &ovenType)
{
    return getSimpleInteger(connectionName, "OvenTypes", "type", ovenType, "id", true);
}

int CharcoalDbHelpers::getOvenTypeIdFromName(const QString &connectionName,
                                             const QString &name)
{
    return getSimpleInteger(connectionName, "OvenTypes", "name", name, "id", true);
}

int CharcoalDbHelpers::getEntityIdFromWebId(const QString &connectionName, const int webId,
                                            const bool verbose)
{
    return getSimpleInteger(connectionName, "Entities", "webId", webId, "id", verbose);
}

int CharcoalDbHelpers::getEntityIdFromName(const QString &connectionName, const QString &name,
                                           const bool verbose)
{
    return getSimpleInteger(connectionName, "Entities", "name", name, "id", verbose);
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

int CharcoalDbHelpers::getEventIdFromWebId(const QString &connectionName,
                                           const int webId, const bool verbose)
{
    return getSimpleInteger(connectionName, "Events", "webId", webId, "id", verbose);
}

int CharcoalDbHelpers::getEventId(const QString &connectionName,
                                  const int entityId,
                                  const int eventTypeId,
                                  qint64 timestamp,
                                  const bool verbose)
{
    return getInteger(connectionName, "Events",
                      { "entityId", "typeId", "date" },
                      { entityId, eventTypeId, timestamp },
                      "id", "AND properties IS NOT NULL", verbose);
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
                                        const bool verbose)
{
    int result = -1;
    const QString queryString(QString("SELECT %1 FROM %2 WHERE %3=:value").arg(
        returnColumn, table, matchColumn));

    QSqlQuery query(QString(), db::Helpers::databaseConnection(connectionName));
    query.prepare(queryString);
    query.bindValue(":value", matchValue);

    if (query.exec() && query.next()) {
        result = query.value(returnColumn).toInt();
    } else if (verbose) {
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
    return getSimpleString(connectionName, "Parcels", "id", id, "code", true);
}

QString CharcoalDbHelpers::getVillageName(const QString &connectionName, const int id)
{
    return getSimpleString(connectionName, "Villages", "id", id, "name", true);
}

QString CharcoalDbHelpers::getTreeSpeciesName(const QString &connectionName, const int id)
{
    return getSimpleString(connectionName, "TreeSpecies", "id", id, "name", true);
}

QString CharcoalDbHelpers::getOvenLetter(const QString &connectionName, const int ovenId)
{
    return getSimpleString(connectionName, "Ovens", "id", ovenId, "name");
}

QString CharcoalDbHelpers::getEventType(const QString &connectionName, const int typeId)
{
    const Enums::SupplyChainAction action = actionById(connectionName, typeId);
    return m_supplyActionMap.value(action);
}

QString CharcoalDbHelpers::getEntityName(const QString &connectionName, const int entityId)
{
    return getSimpleString(connectionName, "Entities", "id", entityId, "name");
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
                                           const bool verbose)
{
    QString result;
    const QString queryString(QString("SELECT %1 FROM %2 WHERE %3=:value").arg(
        returnColumn, table, matchColumn));

    QSqlQuery query(QString(), db::Helpers::databaseConnection(connectionName));
    query.prepare(queryString);
    query.bindValue(":value", matchValue);

    if (query.exec() && query.next()) {
        result = query.value(returnColumn).toString();
    } else if (verbose) {
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
                                      "id", actionId, "actionName", true));
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
                                      "actionName", m_supplyActionMap.value(action),
                                      "id", true));

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
                                       "id", typeId, "name", true));
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
                                      "name", m_packageTypeMap.value(type),
                                      "id", true));

    if (type == Enums::PackageType::Unknown || typeId == -1) {
        qWarning() << RED("Invalid type ID!") << typeId << type;
        return Enums::PackageType::Unknown;
    }

    m_packageTypeDbMap.insert(typeId, type);

    return type;
}
