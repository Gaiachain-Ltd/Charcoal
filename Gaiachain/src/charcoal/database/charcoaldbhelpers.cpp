#include "charcoaldbhelpers.h"

#include "common/logs.h"
#include "database/dbhelpers.h"

#include <QMetaEnum>
#include <QString>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

QString CharcoalDbHelpers::getPlotId(const QString &packageId)
{
        const QStringList parts(packageId.split(sep));

    if (parts.length() < 3) {
        qWarning() << RED("Invalid ID passed to getPlotId") << packageId;
        return QString();
    }

    const QStringList plot(parts.mid(0, 3));
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
                   << "SQL error:" << query.lastError()
                   << "For query:" << query.lastQuery();
    }

    return result;
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
                   << "SQL error:" << query.lastError()
                   << "For query:" << query.lastQuery();
    }

    return result;
}
