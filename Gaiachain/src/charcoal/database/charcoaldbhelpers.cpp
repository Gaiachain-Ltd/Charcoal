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
    switch (action) {
    case Enums::SupplyChainAction::LoggingBeginning:
        return QStringLiteral("LB");
    case Enums::SupplyChainAction::LoggingEnding:
        return QStringLiteral("LE");
    case Enums::SupplyChainAction::CarbonizationBeginning:
        return QStringLiteral("CB");
    case Enums::SupplyChainAction::CarbonizationEnding:
        return QStringLiteral("CE");
    case Enums::SupplyChainAction::LoadingAndTransport:
        return QStringLiteral("TR");
    case Enums::SupplyChainAction::Reception:
        return QStringLiteral("RE");
    default: return QString();
    }

    return QString();
}

Enums::SupplyChainAction CharcoalDbHelpers::actionById(const QString &connectionName, const int id)
{
    const QString name(getEventType(connectionName, id));

    if (name == QStringLiteral("LB")) {
        return Enums::SupplyChainAction::LoggingBeginning;
    } else if (name == QStringLiteral("LE")) {
        return Enums::SupplyChainAction::LoggingEnding;
    } else if (name == QStringLiteral("CB")) {
        return Enums::SupplyChainAction::CarbonizationBeginning;
    } else if (name == QStringLiteral("CE")) {
        return Enums::SupplyChainAction::CarbonizationEnding;
    } else if (name == QStringLiteral("TR")) {
        return Enums::SupplyChainAction::LoadingAndTransport;
    } else if (name == QStringLiteral("RE")) {
        return Enums::SupplyChainAction::Reception;
    }

    return Enums::SupplyChainAction::Unknown;
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

int CharcoalDbHelpers::getEntityTypeId(const QString &connectionName, const Enums::PackageType type)
{
    const QString typeString(QMetaEnum::fromType<Enums::PackageType>()
                                 .valueToKey(int(type)));

    return getSimpleInteger(connectionName, "EntityTypes", "name", typeString, "id");
}

int CharcoalDbHelpers::getEventTypeId(const QString &connectionName,
                                      const Enums::SupplyChainAction action)
{
    const QString typeString(actionAbbreviation(action));
    return getSimpleInteger(connectionName, "EventTypes", "actionName", typeString, "id");
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
    return getSimpleInteger(connectionName, "EventTypes", "actionName", action, "id");
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
    return getSimpleString(connectionName, "EventTypes", "id", typeId, "actionName");
}

QString CharcoalDbHelpers::getEntityName(const QString &connectionName, const int entityId)
{
    return getSimpleString(connectionName, "Entities", "id", entityId, "name");
}

Enums::PackageType CharcoalDbHelpers::getEntityType(const QString &connectionName,
                                                    const int typeId)
{
    const QString type(getSimpleString(connectionName, "EntityTypes",
                                       "id", typeId, "name", true).toLower());
    if (type == QStringLiteral("plot")) {
        return Enums::PackageType::Plot;
    } else if (type == QStringLiteral("harvest")) {
        return Enums::PackageType::Harvest;
    } else if (type == QStringLiteral("transport")) {
        return Enums::PackageType::Transport;
    }

    return Enums::PackageType::Unknown;
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
