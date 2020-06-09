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

QString EntitiesModel::getPlotId(const QString &id) const
{
    const QStringList parts(id.split(sep));

    if (parts.length() < 3) {
        qWarning() << RED("Invalid ID passed to getPlotId") << id;
        return QString();
    }

    const QStringList plot(parts.mid(0, 3));
    return plot.join(sep);
}

QString EntitiesModel::getTransportIdFromBags(const QVariantList &scannedQrs) const
{
    Q_UNUSED(scannedQrs)

    // TODO!
    return QString();
}

int EntitiesModel::nextTransportNumber(const QString &harvestId) const
{
    const QString plotId(getPlotId(harvestId));

    return -1;
}

int EntitiesModel::bagCountInTransport(const QString &transportId) const
{
    Q_UNUSED(transportId)

    // TODO!
    return -1;
}

QString EntitiesModel::plateNumberInTransport(const QString &transportId) const
{
    Q_UNUSED(transportId)

    // TODO!
    return QString();
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

    query.prepare("INSERT INTO Entities (typeId, name, isFinished, isCommitted, isReplanted) "
                  "VALUES (:typeId, :plotId, 0, 0, 0)");
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

    query.prepare("INSERT INTO Events (entityId, typeId, userId,"
                  "date, locationLatitude, locationLongitude, properties) "
                  "VALUES (:entityId, :typeId, :userId, :date, "
                  ":locationLatitude, :locationLongitude, :properties)");
    query.bindValue(":entityId", entityId);
    query.bindValue(":typeId", eventTypeId);
    query.bindValue(":userId", userId);
    query.bindValue(":date", timestamp);
    query.bindValue(":locationLatitude", coordinate.latitude());
    query.bindValue(":locationLongitude", coordinate.longitude());
    // TODO: use Tags to denote the properties more reliably!
    query.bindValue(":properties", QVariantMap {
                                       { "parcel", parcel },
                                       { "village", village },
                                       { "treeSpecies", treeSpecies }
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
    query.prepare("INSERT INTO Events (entityId, typeId, userId,"
                  "date, locationLatitude, locationLongitude, properties) "
                  "VALUES (:entityId, :typeId, :userId, :date, "
                  ":locationLatitude, :locationLongitude, :properties)");
    query.bindValue(":entityId", entityId);
    query.bindValue(":typeId", eventTypeId);
    query.bindValue(":userId", userId);
    query.bindValue(":date", timestamp);
    query.bindValue(":locationLatitude", coordinate.latitude());
    query.bindValue(":locationLongitude", coordinate.longitude());
    // TODO: use Tags to denote the properties more reliably!
    query.bindValue(":properties", QVariantMap {
                                       { "numberOfTrees", numberOfTrees }
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
    const QString parentEntityId(findEntityId(plotId));

    if (typeId.isEmpty()) {
        qWarning() << RED("Harvest ID type not found!");
        return;
    }

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));

    query.prepare("INSERT INTO Entities (typeId, name, parent, isFinished, isCommitted, isReplanted) "
                  "VALUES (:typeId, :harvestId, :parent, 0, 0, 0)");
    query.bindValue(":typeId", typeId);
    query.bindValue(":harvestId", harvestId);
    query.bindValue(":parent", parentEntityId);

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

    query.prepare("INSERT INTO Events (entityId, typeId, userId,"
                  "date, locationLatitude, locationLongitude, properties) "
                  "VALUES (:entityId, :typeId, :userId, :date, "
                  ":locationLatitude, :locationLongitude, :properties)");
    query.bindValue(":entityId", entityId);
    query.bindValue(":typeId", eventTypeId);
    query.bindValue(":userId", userId);
    query.bindValue(":date", timestamp);
    query.bindValue(":locationLatitude", coordinate.latitude());
    query.bindValue(":locationLongitude", coordinate.longitude());
    // TODO: use Tags to denote the properties more reliably!
    query.bindValue(":properties", QVariantMap {
                                       { "plotId", plotId },
                                       { "ovenId", ovenId },
                                       { "ovenType", ovenType },
                                       { "ovenDimensions", ovenDimensions }
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
    const QString &ovenIds) const
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
    query.prepare("INSERT INTO Events (entityId, typeId, userId,"
                  "date, locationLatitude, locationLongitude, properties) "
                  "VALUES (:entityId, :typeId, :userId, :date, "
                  ":locationLatitude, :locationLongitude, :properties)");
    query.bindValue(":entityId", entityId);
    query.bindValue(":typeId", eventTypeId);
    query.bindValue(":userId", userId);
    query.bindValue(":date", timestamp);
    query.bindValue(":locationLatitude", coordinate.latitude());
    query.bindValue(":locationLongitude", coordinate.longitude());
    // TODO: use Tags to denote the properties more reliably!
    query.bindValue(":properties", QVariantMap {
                                       { "plotId", plotId },
                                       { "ovenIds", ovenIds }
                                   });

    if (query.exec() == false) {
        qWarning() << RED("Inserting Logging Ending event has failed!")
                   << query.lastError().text() << "for query:" << query.lastQuery();
        return;
    }

    // Lastly, send a request to server to add it, too.
}

void EntitiesModel::registerTransportAndLoading(
    const QGeoCoordinate &coordinate, const QDateTime &timestamp,
    const QString &userId, const QString &transportId, const QString &harvestId,
    const QString &plateNumber, const QString &destination,
    const QVariantList &scannedQrs) const
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
    const QString parentEntityId(findEntityId(getPlotId(transportId)));

    if (typeId.isEmpty()) {
        qWarning() << RED("Transport ID type not found!");
        return;
    }

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));

    query.prepare("INSERT INTO Entities (typeId, name, parent, isFinished, isCommitted, isReplanted) "
                  "VALUES (:typeId, :transportId, :parent, 0, 0, 0)");
    query.bindValue(":typeId", typeId);
    query.bindValue(":transportId", transportId);
    query.bindValue(":parent", parentEntityId);

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

    query.prepare("INSERT INTO Events (entityId, typeId, userId,"
                  "date, locationLatitude, locationLongitude, properties) "
                  "VALUES (:entityId, :typeId, :userId, :date, "
                  ":locationLatitude, :locationLongitude, :properties)");
    query.bindValue(":entityId", entityId);
    query.bindValue(":typeId", eventTypeId);
    query.bindValue(":userId", userId);
    query.bindValue(":date", timestamp);
    query.bindValue(":locationLatitude", coordinate.latitude());
    query.bindValue(":locationLongitude", coordinate.longitude());
    // TODO: use Tags to denote the properties more reliably!
    query.bindValue(":properties", QVariantMap {
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

void EntitiesModel::registerReception(
    const QGeoCoordinate &coordinate, const QDateTime &timestamp,
    const QString &userId, const QString &transportId,
    const QVariantList &documents, const QVariantList &receipts,
    const QVariantList &scannedQrs) const
{
    /*
     * Algorithm is:
     * - find entity ID in table (created in Loading and Transport step)
     * - insert event into table
     * - mark PlotId, HarvestId and TransportId as finished
     * - send action to web server
     */

    qDebug() << "Registering reception" << coordinate << timestamp
             << userId << transportId << documents << receipts
             << scannedQrs.size();

    const QString entityId(findEntityId(transportId));

    if (entityId.isEmpty()) {
        qWarning() << RED("Entity ID not found!");
        return;
    }

    const QString eventTypeId(findEventTypeId(Enums::SupplyChainAction::Reception));

    if (eventTypeId.isEmpty()) {
        qWarning() << RED("Event Type ID not found!");
        return;
    }

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));
    query.prepare("INSERT INTO Events (entityId, typeId, userId,"
                  "date, locationLatitude, locationLongitude, properties) "
                  "VALUES (:entityId, :typeId, :userId, :date, "
                  ":locationLatitude, :locationLongitude, :properties)");
    query.bindValue(":entityId", entityId);
    query.bindValue(":typeId", eventTypeId);
    query.bindValue(":userId", userId);
    query.bindValue(":date", timestamp);
    query.bindValue(":locationLatitude", coordinate.latitude());
    query.bindValue(":locationLongitude", coordinate.longitude());
    // TODO: use Tags to denote the properties more reliably!
    query.bindValue(":properties", QVariantMap {
                                       { "transportId", transportId },
                                       { "documents", documents },
                                       { "receipts", receipts },
                                       { "scannedQrs", scannedQrs }
                                   });

    if (query.exec() == false) {
        qWarning() << RED("Inserting reception event has failed!")
                   << query.lastError().text() << "for query:" << query.lastQuery();
        return;
    }

    // Lastly, send a request to server to add it, too.
}

void EntitiesModel::finalizeSupplyChain(const QString &plotId) const
{
    const QString parentId(findEntityId(plotId));
    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));
    query.prepare("UPDATE Entities SET isFinished=1 WHERE name=:plotId OR parentName=:parentId");
    query.bindValue(":plotId", plotId);
    query.bindValue(":parentId", parentId);

    if (query.exec() == false) {
        qWarning() << RED("Finishing a supply chain has failed!")
                   << query.lastError().text() << "for query:" << query.lastQuery();
        return;
    }
}

void EntitiesModel::registerReplantation(
    const QGeoCoordinate &coordinate, const QDateTime &timestamp,
    const QString &userId, const QString &plotId,
    const int numberOfTrees, const QString &treeSpecies,
    const QDateTime &beginningDate) const
{
    /*
     * Algorithm is:
     * - insert replantation entry into table
     * - mark parent entity (plot) as replanted (set isReplanted to 1)
     * - send action to web server
     */

    qDebug() << "Registering replantation" << coordinate << timestamp
             << userId << plotId << numberOfTrees << treeSpecies
             << beginningDate;

    const QString parentId(findEntityId(plotId));
    const QString treeSpeciesId(findTreeSpeciesId(treeSpecies));

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));

    query.prepare("INSERT INTO Replantations (plotId, userId, "
                  "numberOfTrees, treeSpecies, "
                  "locationLatitude, locationLongitude, "
                  "beginningDate, endingDate) "
                  "VALUES (:plotId, :userId, "
                  ":numberOfTrees, :treeSpecies, "
                  ":locationLatitude, :locationLongitude, "
                  ":beginningDate, :endingDate)");
    query.bindValue(":plotId", parentId);
    query.bindValue(":userId", userId);
    query.bindValue(":numberOfTrees", numberOfTrees);
    query.bindValue(":treeSpecies", treeSpeciesId);
    query.bindValue(":locationLatitude", coordinate.latitude());
    query.bindValue(":locationLongitude", coordinate.longitude());
    query.bindValue(":beginningDate", beginningDate);
    query.bindValue(":endingDate", timestamp);

    if (query.exec() == false) {
        qWarning() << RED("Inserting Replantation has failed!")
                   << query.lastError().text()
                   << "for query:" << query.lastQuery()
                   << "DB:" << m_dbConnName;
        return;
    }

    // Mark parent Entity as replanted
    query.prepare("UPDATE Entities SET isReplanted=1 WHERE name=:plotId");
    query.bindValue(":plotId", plotId);

    if (query.exec() == false) {
        qWarning() << RED("Finishing replantation of a plot has failed!")
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

QString EntitiesModel::findTreeSpeciesId(const QString &species) const
{
    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));

    query.prepare("SELECT id FROM TreeSpecies WHERE name=:species");
    query.bindValue(":species", species);

    if (query.exec()) {
        query.next();
        return query.value("id").toString();
    }

    qWarning() << RED("Getting TreeSpecies has failed!")
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
