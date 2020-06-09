#include "entitiesmodel.h"

#include "database/dbhelpers.h"
#include "common/logs.h"

#include <QGeoCoordinate>

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

#include <QMetaEnum>
#include <QDateTime>
#include <QDate>

EntitiesModel::EntitiesModel(QObject *parent) : QSqlQueryModel(parent)
{
}

void EntitiesModel::setDbConnection(const QString &connectionName)
{

    m_dbConnName = connectionName;

    setQuery("SELECT name FROM Entities", db::Helpers::databaseConnection(connectionName));

    // TODO: we need to also return Events for each Entity!
}

QString EntitiesModel::generatePlotId(const QString &userId,
                                      const QString &parcelCode,
                                      const QDate &date) const
{
    return userId + sep + parcelCode + sep + date.toString(dateFormat);
}

QString EntitiesModel::generateHarvestId(const QString &plotId,
                                         const QString &userId) const
{
    return plotId + sep + userId;
}

QString EntitiesModel::generateTransportId(const QString &harvestId,
                                           const QString &licensePlate,
                                           const int transportNumber,
                                           const QDate &date) const
{
    return harvestId + sep + licensePlate
        + sep + "T" + QString::number(transportNumber)
            + sep + date.toString(dateFormat);
}

QString EntitiesModel::getPlotId(const QString &harvestId) const
{
    QStringList parts(harvestId.split(sep));
    parts.removeLast();
    return parts.join(sep);
}

void EntitiesModel::registerLoggingBeginning(
    const QGeoCoordinate &coordinate,
    const QDateTime &timestamp, const QString &userId,
    const QString &parcel, const QString &village,
    const QString &treeSpecies) const
{
    /*
     * Algorithm is:
     * - insert entity into table
     * - get new entity's ID
     * - insert event into table
     * - send action to web server
     */

    qDebug() << "Registering logging beginning" << coordinate << timestamp
             << parcel << userId << village << treeSpecies;

    // First, insert a new Entity into table
    const QString plotId(generatePlotId(userId, parcel, timestamp.date()));
    const QString typeId(findEntityTypeId(Enums::PackageType::Plot));

    if (typeId.isEmpty()) {
        qWarning() << RED("Plot ID type not found!");
        return;
    }

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));

    query.prepare("INSERT INTO Entities (typeId, name, isUsed, isCommitted) "
                  "VALUES (:typeId, :plotId, 0, 0)");
    query.bindValue(":typeId", typeId);
    query.bindValue(":plotId", plotId);

    if (query.exec() == false) {
        qWarning() << RED("Inserting Plot entity has failed!")
                   << query.lastError().text()
                   << "for query:" << query.lastQuery()
                   << "DB:" << m_dbConnName;
        return;
    }

    // Then, insert a new Event under that Entity
    const QString entityId(query.lastInsertId().toString());
    const QString eventTypeId(findEventTypeId(Enums::SupplyChainAction::LoggingBeginning));

    if (eventTypeId.isEmpty()) {
        qWarning() << RED("Event Type ID not found!");
        return;
    }

    query.prepare("INSERT INTO Events (entityId, typeId, "
                  "date, locationLatitude, locationLongitude, properties) "
                  "VALUES (:entityId, :typeId, :date, :locationLatitude, :locationLongitude,"
                  ":properties)");
    query.bindValue(":entityId", entityId);
    query.bindValue(":typeId", eventTypeId);
    query.bindValue(":date", timestamp);
    query.bindValue(":locationLatitude", coordinate.latitude());
    query.bindValue(":locationLongitude", coordinate.longitude());
    // TODO: use Tags to denote the properties more reliably!
    query.bindValue(":properties", QVariantMap {
                                       { "parcel", parcel },
                                       { "village", village },
                                       { "treeSpecies", treeSpecies },
                                       { "userId", userId }
                                   });

    if (query.exec() == false) {
        qWarning() << RED("Inserting Logging Beginning event has failed!")
                   << query.lastError().text() << "for query:" << query.lastQuery();
        return;
    }

    // Lastly, send a request to server to add it, too.
}

void EntitiesModel::registerLoggingEnding(
    const QGeoCoordinate &coordinate, const QDateTime &timestamp,
    const QString &userId, const QString &plotId,
    const int numberOfTrees) const
{
    /*
     * Algorithm is:
     * - find entity ID in table (created in Logging Beginning step)
     * - insert event into table
     * - send action to web server
     */

    qDebug() << "Registering logging ending" << coordinate << timestamp
             << plotId << userId << numberOfTrees;

    const QString entityId(findEntityId(plotId));

    if (entityId.isEmpty()) {
        qWarning() << RED("Entity ID not found!");
        return;
    }

    const QString eventTypeId(findEventTypeId(Enums::SupplyChainAction::LoggingEnding));

    if (eventTypeId.isEmpty()) {
        qWarning() << RED("Event Type ID not found!");
        return;
    }

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));
    query.prepare("INSERT INTO Events (entityId, typeId, "
                  "date, locationLatitude, locationLongitude, properties) "
                  "VALUES (:entityId, :typeId, :date, :locationLatitude, :locationLongitude,"
                  ":properties)");
    query.bindValue(":entityId", entityId);
    query.bindValue(":typeId", eventTypeId);
    query.bindValue(":date", timestamp);
    query.bindValue(":locationLatitude", coordinate.latitude());
    query.bindValue(":locationLongitude", coordinate.longitude());
    // TODO: use Tags to denote the properties more reliably!
    query.bindValue(":properties", QVariantMap {
                                       { "numberOfTrees", numberOfTrees },
                                       { "userId", userId }
                                   });

    if (query.exec() == false) {
        qWarning() << RED("Inserting Logging Ending event has failed!")
                   << query.lastError().text() << "for query:" << query.lastQuery();
        return;
    }
}

void EntitiesModel::registerCarbonizationBeginning(
    const QGeoCoordinate &coordinate, const QDateTime &timestamp,
    const QString &userId, const QString &plotId, const QString &ovenId,
    const QString &ovenType, const QVariantMap &ovenDimensions) const
{
    /*
     * Algorithm is:
     * - insert entity into table
     * - get new entity's ID
     * - insert event into table
     * - send action to web server
     */

    qDebug() << "Registering carbonization beginning" << coordinate << timestamp
             << plotId << ovenId << userId << ovenType << ovenDimensions;

    // First, insert a new Entity into table
    const QString harvestId(generateHarvestId(plotId, userId));
    const QString typeId(findEntityTypeId(Enums::PackageType::Harvest));

    if (typeId.isEmpty()) {
        qWarning() << RED("Harvest ID type not found!");
        return;
    }

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));

    query.prepare("INSERT INTO Entities (typeId, name, isUsed, isCommitted) "
                  "VALUES (:typeId, :harvestId, 0, 0)");
    query.bindValue(":typeId", typeId);
    query.bindValue(":harvestId", harvestId);

    if (query.exec() == false) {
        qWarning() << RED("Inserting Carbonization entity has failed!")
                   << query.lastError().text()
                   << "for query:" << query.lastQuery()
                   << "DB:" << m_dbConnName;
        return;
    }

    // Then, insert a new Event under that Entity
    const QString entityId(query.lastInsertId().toString());
    const QString eventTypeId(findEventTypeId(Enums::SupplyChainAction::CarbonizationBeginning));

    if (eventTypeId.isEmpty()) {
        qWarning() << RED("Event Type ID not found!");
        return;
    }

    query.prepare("INSERT INTO Events (entityId, typeId, "
                  "date, locationLatitude, locationLongitude, properties) "
                  "VALUES (:entityId, :typeId, :date, :locationLatitude, :locationLongitude,"
                  ":properties)");
    query.bindValue(":entityId", entityId);
    query.bindValue(":typeId", eventTypeId);
    query.bindValue(":date", timestamp);
    query.bindValue(":locationLatitude", coordinate.latitude());
    query.bindValue(":locationLongitude", coordinate.longitude());
    // TODO: use Tags to denote the properties more reliably!
    query.bindValue(":properties", QVariantMap {
                                       { "plotId", plotId },
                                       { "ovenId", ovenId },
                                       { "ovenType", ovenType },
                                       { "ovenDimensions", ovenDimensions },
                                       { "userId", userId }
                                   });

    if (query.exec() == false) {
        qWarning() << RED("Inserting Carbonization Beginning event has failed!")
                   << query.lastError().text() << "for query:" << query.lastQuery();
        return;
    }

    // Lastly, send a request to server to add it, too.
}

void EntitiesModel::registerCarbonizationEnding(
    const QGeoCoordinate &coordinate, const QDateTime &timestamp,
    const QString &userId, const QString &harvestId, const QString &plotId,
    const QVariantList &ovenIds) const
{
    /*
     * Algorithm is:
     * - find entity ID in table (created in Carbonization Beginning step)
     * - insert event into table
     * - send action to web server
     */

    qDebug() << "Registering carbonization ending" << coordinate << timestamp
             << harvestId << plotId << ovenIds << userId;

    const QString entityId(findEntityId(harvestId));

    if (entityId.isEmpty()) {
        qWarning() << RED("Entity ID not found!");
        return;
    }

    const QString eventTypeId(findEventTypeId(Enums::SupplyChainAction::CarbonizationEnding));

    if (eventTypeId.isEmpty()) {
        qWarning() << RED("Event Type ID not found!");
        return;
    }

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));
    query.prepare("INSERT INTO Events (entityId, typeId, "
                  "date, locationLatitude, locationLongitude, properties) "
                  "VALUES (:entityId, :typeId, :date, :locationLatitude, :locationLongitude,"
                  ":properties)");
    query.bindValue(":entityId", entityId);
    query.bindValue(":typeId", eventTypeId);
    query.bindValue(":date", timestamp);
    query.bindValue(":locationLatitude", coordinate.latitude());
    query.bindValue(":locationLongitude", coordinate.longitude());
    // TODO: use Tags to denote the properties more reliably!
    query.bindValue(":properties", QVariantMap {
                                       { "plotId", plotId },
                                       { "ovenIds", ovenIds },
                                       { "userId", userId }
                                   });

    if (query.exec() == false) {
        qWarning() << RED("Inserting Logging Ending event has failed!")
                   << query.lastError().text() << "for query:" << query.lastQuery();
        return;
    }
}

void EntitiesModel::registerTransportAndLoading(
    const QGeoCoordinate &coordinate, const QDateTime &timestamp,
    const QString &userId, const QString &transportId, const QString &harvestId,
    const QString &plateNumber, const QString &destination,
    const QVariantMap &scannedQrs) const
{
    /*
     * Algorithm is:
     * - insert entity into table
     * - get new entity's ID
     * - insert event into table
     * - send action to web server
     */

    qDebug() << "Registering transport and loading" << coordinate << timestamp
             << userId << transportId << harvestId << plateNumber
             << destination << scannedQrs.size();

    // First, insert a new Entity into table
    const QString typeId(findEntityTypeId(Enums::PackageType::Transport));

    if (typeId.isEmpty()) {
        qWarning() << RED("Transport ID type not found!");
        return;
    }

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));

    query.prepare("INSERT INTO Entities (typeId, name, isUsed, isCommitted) "
                  "VALUES (:typeId, :transportId, 0, 0)");
    query.bindValue(":typeId", typeId);
    query.bindValue(":transportId", transportId);

    if (query.exec() == false) {
        qWarning() << RED("Inserting Loading and Transport entity has failed!")
                   << query.lastError().text()
                   << "for query:" << query.lastQuery()
                   << "DB:" << m_dbConnName;
        return;
    }

    // Then, insert a new Event under that Entity
    const QString entityId(query.lastInsertId().toString());
    const QString eventTypeId(findEventTypeId(Enums::SupplyChainAction::LoadingAndTransport));

    if (eventTypeId.isEmpty()) {
        qWarning() << RED("Event Type ID not found!");
        return;
    }

    query.prepare("INSERT INTO Events (entityId, typeId, "
                  "date, locationLatitude, locationLongitude, properties) "
                  "VALUES (:entityId, :typeId, :date, :locationLatitude, :locationLongitude,"
                  ":properties)");
    query.bindValue(":entityId", entityId);
    query.bindValue(":typeId", eventTypeId);
    query.bindValue(":date", timestamp);
    query.bindValue(":locationLatitude", coordinate.latitude());
    query.bindValue(":locationLongitude", coordinate.longitude());
    // TODO: use Tags to denote the properties more reliably!
    query.bindValue(":properties", QVariantMap {
                                       { "userId", userId },
                                       { "harvestId", harvestId },
                                       { "plateNumber", plateNumber },
                                       { "destination", destination },
                                       { "scannedQrs", scannedQrs }
                                   });

    if (query.exec() == false) {
        qWarning() << RED("Inserting Carbonization Beginning event has failed!")
                   << query.lastError().text() << "for query:" << query.lastQuery();
        return;
    }

    // Lastly, send a request to server to add it, too.
}

/*!
 * Returns Entity entry ID (from DB) for package of given \a name, where \a name
 * is PlotId, HarvestId or TransportId.
 */
QString EntitiesModel::findEntityId(const QString &name) const
{
    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));

    query.prepare("SELECT id FROM Entities WHERE name=:nameString");
    query.bindValue(":nameString", name);

    if (query.exec()) {
        query.next();
        return query.value("id").toString();
    }

    qWarning() << RED("Getting Entity ID has failed!")
               << query.lastError().text()
               << "for query:" << query.lastQuery()
               << "DB:" << m_dbConnName;

    return QString();
}

QString EntitiesModel::findEntityTypeId(const Enums::PackageType type) const
{
    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));
    const QString typeString(QMetaEnum::fromType<Enums::PackageType>()
                                 .valueToKey(int(type)));

    query.prepare("SELECT id FROM EntityTypes WHERE name=:typeString");
    query.bindValue(":typeString", typeString);

    if (query.exec()) {
        query.next();
        return query.value("id").toString();
    }

    qWarning() << RED("Getting EntityType has failed!")
               << query.lastError().text()
               << "for query:" << query.lastQuery()
               << "DB:" << m_dbConnName;

    return QString();
}

QString EntitiesModel::findEventTypeId(const Enums::SupplyChainAction action) const
{
    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));
    const QString typeString(actionAbbreviation(action));

    query.prepare("SELECT id FROM EventTypes WHERE actionName=:typeString");
    query.bindValue(":typeString", typeString);

    if (query.exec()) {
        query.next();
        return query.value("id").toString();
    }

    qWarning() << RED("Getting EventType has failed!")
               << query.lastError().text()
               << "for query:" << query.lastQuery()
               << "DB:" << m_dbConnName;

    return QString();
}

QString EntitiesModel::actionAbbreviation(const Enums::SupplyChainAction action) const
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
