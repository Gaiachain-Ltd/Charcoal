#include "actioncontroller.h"

#include "database/dbhelpers.h"
#include "common/logs.h"

#include <QGeoCoordinate>

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QMetaEnum>
#include <QDateTime>
#include <QDate>

ActionController::ActionController(QObject *parent) : QObject(parent)
{
}

void ActionController::setDbConnection(const QString &connectionName)
{
    m_dbConnName = connectionName;
}

QString ActionController::generatePlotId(const QString &userId,
                                      const QString &parcelCode,
                                      const QDate &date) const
{
    return userId + sep + parcelCode + sep + date.toString(dateFormat);
}

QString ActionController::generateHarvestId(const QString &plotId,
                                         const QString &userId) const
{
    return plotId + sep + userId;
}

QString ActionController::generateTransportId(const QString &harvestId,
                                           const QString &licensePlate,
                                           const int transportNumber,
                                           const QDate &date) const
{
    return harvestId + sep + licensePlate
        + sep + "T" + QString::number(transportNumber)
            + sep + date.toString(dateFormat);
}

QString ActionController::getPlotId(const QString &id) const
{
    const QStringList parts(id.split(sep));

    if (parts.length() < 3) {
        qWarning() << RED("Invalid ID passed to getPlotId") << id;
        return QString();
    }

    const QStringList plot(parts.mid(0, 3));
    return plot.join(sep);
}

QString ActionController::getTransportIdFromBags(const QVariantList &scannedQrs) const
{
    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));

    query.prepare("SELECT properties, entityId FROM Events "
                  "WHERE entityId IN (SELECT id FROM Entities WHERE isFinished=0)");

    QString transportEntityId;
    if (query.exec()) {
        while (query.next()) {
            const QByteArray propertiesString(query.value("properties").toByteArray());
            const QJsonDocument propertiersJson(QJsonDocument::fromJson(propertiesString));
            const QVariantMap properties(propertiersJson.toVariant().toMap());
            const QVariantList qrs(properties.value("scannedQrs").toList());

            for (const QVariant &qr : scannedQrs) {
                if (qrs.contains(qr)) {
                    transportEntityId = query.value("entityId").toString();
                    //qDebug() << "HIT!" << qr.toString() << transportEntityId;
                    break;
                }
            }

            if (transportEntityId.isEmpty() == false) {
                break;
            }
        }
    } else {
        qWarning() << RED("Getting transport ID from bags has failed!")
                   << query.lastError().text()
                   << "for query:" << query.lastQuery()
                   << "DB:" << m_dbConnName;
    }

    if (transportEntityId.isEmpty()) {
        qWarning() << RED("No transport matching scanned bags has been found!")
                   << scannedQrs;
        return QString();
    }

    query.prepare("SELECT name FROM Entities WHERE id=:transportEntityId");
    query.bindValue(":transportEntityId", transportEntityId);

    if (query.exec()) {
        query.next();
        return query.value("name").toString();
    }

    qWarning() << RED("Getting Transport ID for bags has failed!")
               << query.lastError().text()
               << "for query:" << query.lastQuery()
               << "DB:" << m_dbConnName
               << scannedQrs;

    return QString();
}

int ActionController::nextTransportNumber(const QString &harvestId) const
{
    const QString plotId(getPlotId(harvestId));
    const QString parentEntityId(findEntityId(plotId));
    const QString transportTypeId(findEntityTypeId(Enums::PackageType::Transport));

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));

    query.prepare("SELECT id FROM Entities WHERE parent=:parentEntityId "
                  "AND typeId=:transportTypeId");
    query.bindValue(":parentEntityId", parentEntityId);
    query.bindValue(":transportTypeId", transportTypeId);

    if (query.exec()) {
        // We can't use query.size() because SQLITE does not support it
        int size = 0;
        while (query.next()) {
            size++;
        }
        size++;

        return size;
    }

    qWarning() << RED("Getting next transport number has failed!")
               << query.lastError().text()
               << "for query:" << query.lastQuery()
               << "DB:" << m_dbConnName;
    return -1;
}

int ActionController::bagCountInTransport(const QString &transportId) const
{
    const QString transportEntityId(findEntityId(transportId));

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));

    query.prepare("SELECT properties FROM Events WHERE entityId=:transportEntityId");
    query.bindValue(":transportEntityId", transportEntityId);

    if (query.exec()) {
        query.next();
        const QByteArray propertiesString(query.value("properties").toByteArray());
        const QJsonDocument propertiersJson(QJsonDocument::fromJson(propertiesString));
        const QVariantMap properties(propertiersJson.toVariant().toMap());
        //qDebug() << "Plate number?" << properties;
        // TODO: use Tags!
        return properties.value("scannedQrs").toList().size();
    }

    qWarning() << RED("Getting bag count has failed!")
               << query.lastError().text()
               << "for query:" << query.lastQuery()
               << "DB:" << m_dbConnName;

    return -1;
}

QString ActionController::plateNumberInTransport(const QString &transportId) const
{
    const QString transportEntityId(findEntityId(transportId));

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));

    query.prepare("SELECT properties FROM Events WHERE entityId=:transportEntityId");
    query.bindValue(":transportEntityId", transportEntityId);

    if (query.exec()) {
        query.next();
        const QByteArray propertiesString(query.value("properties").toByteArray());
        const QJsonDocument propertiersJson(QJsonDocument::fromJson(propertiesString));
        const QVariantMap properties(propertiersJson.toVariant().toMap());
        //qDebug() << "Plate number?" << properties;
        // TODO: use Tags!
        return properties.value("plateNumber").toString();
    }

    qWarning() << RED("Getting plate number has failed!")
               << query.lastError().text()
               << "for query:" << query.lastQuery()
               << "DB:" << m_dbConnName;

    return QString();
}

int ActionController::scannedBagsCount(const QString &transportId) const
{
    qDebug() << RED("TODO: fix") << Q_FUNC_INFO;
    return scannedBagsForAction(transportId, Enums::SupplyChainAction::Reception);
}

int ActionController::scannedBagsTotal(const QString &transportId) const
{
    qDebug() << RED("TODO: fix") << Q_FUNC_INFO;
    return scannedBagsForAction(transportId, Enums::SupplyChainAction::LoadingAndTransport);
}

int ActionController::registeredTrucksCount(const QString &transportId) const
{
    qDebug() << RED("TODO: fix") << Q_FUNC_INFO;
    const QString plotId(getPlotId(transportId));
    const QString transportTypeId(findEntityTypeId(Enums::PackageType::Transport));
    const QString receptionTypeId(findEventTypeId(Enums::SupplyChainAction::Reception));

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));

    // It's a bit "hacky" we count only Reception events, that gives us number
    // of events where transport has left but not arrived yet.
    query.prepare("SELECT COUNT(id) FROM Events WHERE typeId=:receptionTypeId "
                  "AND entityId IN (SELECT id FROM Entities WHERE parent=:plotId "
                  "AND typeId=:transportTypeId)");
    query.bindValue(":receptionTypeId", receptionTypeId);
    query.bindValue(":plotId", plotId);
    query.bindValue(":transportTypeId", transportTypeId);

    if (query.exec() == false) {
        qWarning() << RED("Getting number of received trucks has failed!")
                   << query.lastError().text()
                   << "for query:" << query.lastQuery()
                   << "DB:" << m_dbConnName;
        return -1;
    }

    query.next();
    return query.value(0).toInt();
}

int ActionController::registeredTrucksTotal(const QString &transportId) const
{
    const QString plotId(getPlotId(transportId));
    const QString parentEntityId(findEntityId(plotId));
    const QString transportTypeId(findEntityTypeId(Enums::PackageType::Transport));

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));

    query.prepare("SELECT COUNT(id) FROM Entities WHERE parent=:parentEntityId "
                  "AND typeId=:transportTypeId");
    query.bindValue(":parentEntityId", parentEntityId);
    query.bindValue(":transportTypeId", transportTypeId);

    if (query.exec() == false) {
        qWarning() << RED("Getting total number of trucks has failed!")
                   << query.lastError().text()
                   << "for query:" << query.lastQuery()
                   << "DB:" << m_dbConnName;
        return -1;
    }

    query.next();
    return query.value(0).toInt();
}

QString ActionController::nextOvenNumber(const QString &plotId) const
{
    const QString parentEntityId(findEntityId(plotId));

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));

    query.prepare("SELECT id FROM Ovens WHERE plot=:parentEntityId");
    query.bindValue(":parentEntityId", parentEntityId);

    if (query.exec()) {
        // We can't use query.size() because SQLITE does not support it
        char size = 'A';
        while (query.next()) {
            size++;
        }

        if (size > 'Z') {
            qWarning() << RED("Too many ovens!") << size;
            return "-2";
        }

        return QChar(size);
    }

    qWarning() << RED("Getting next transport number has failed!")
               << query.lastError().text()
               << "for query:" << query.lastQuery()
               << "DB:" << m_dbConnName;

    return "-1";
}

QVariantList ActionController::defaultOvenDimensions(const QString &ovenType) const
{
    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));
    QVariantList dimensions;
    if (ovenType == "metallic") {
        query.prepare("SELECT height, length, width FROM OvenTypes WHERE name=:ovenType");
        query.bindValue(":ovenType", ovenType);

        if (query.exec() == false) {
            qWarning() << RED("Getting metallic oven dimensions has failed!")
                       << query.lastError().text() << "for query:" << query.lastQuery();
            return QVariantList{ 0, 0, 0 };
        }

        query.next();
        dimensions.append(query.value("height").toString());
        dimensions.append(query.value("length").toString());
        dimensions.append(query.value("width").toString());
    } else {
        return QVariantList{ 0, 0, 0 };
    }

    return dimensions;
}

void ActionController::registerLoggingBeginning(
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

    query.prepare("INSERT INTO Entities (typeId, name, isFinished, isReplanted) "
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

    query.prepare("INSERT INTO Events (entityId, typeId, userId,"
                  "date, locationLatitude, locationLongitude, properties, "
                  "isCommitted) "
                  "VALUES (:entityId, :typeId, :userId, :date, "
                  ":locationLatitude, :locationLongitude, :properties, 0)");
    query.bindValue(":entityId", entityId);
    query.bindValue(":typeId", eventTypeId);
    query.bindValue(":userId", userId);
    query.bindValue(":date", timestamp);
    query.bindValue(":locationLatitude", coordinate.latitude());
    query.bindValue(":locationLongitude", coordinate.longitude());
    // TODO: use Tags to denote the properties more reliably!
    query.bindValue(":properties",
                    propertiesToString(QVariantMap {
                        { "parcel", parcel },
                        { "village", village },
                        { "treeSpecies", treeSpecies }
                    }));

    if (query.exec() == false) {
        qWarning() << RED("Inserting Logging Beginning event has failed!")
                   << query.lastError().text() << "for query:" << query.lastQuery();
        return;
    }

    // Lastly, send a request to server to add it, too.
}

void ActionController::registerLoggingEnding(
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
                  "date, locationLatitude, locationLongitude, properties "
                  "isCommitted) "
                  "VALUES (:entityId, :typeId, :userId, :date, "
                  ":locationLatitude, :locationLongitude, :properties, 0)");
    query.bindValue(":entityId", entityId);
    query.bindValue(":typeId", eventTypeId);
    query.bindValue(":userId", userId);
    query.bindValue(":date", timestamp);
    query.bindValue(":locationLatitude", coordinate.latitude());
    query.bindValue(":locationLongitude", coordinate.longitude());
    // TODO: use Tags to denote the properties more reliably!
    query.bindValue(":properties",
                    propertiesToString(QVariantMap {
                        { "numberOfTrees", numberOfTrees }
                    }));

    if (query.exec() == false) {
        qWarning() << RED("Inserting Logging Ending event has failed!")
                   << query.lastError().text() << "for query:" << query.lastQuery();
        return;
    }
}

void ActionController::registerCarbonizationBeginning(
    const QGeoCoordinate &coordinate, const QDateTime &timestamp,
    const QString &userId, const QString &plotId, const QString &ovenId,
    const QString &ovenType, const QVariantList &ovenDimensions) const
{
    /*
     * Algorithm is:
     * - check if harvest ID already exists
     * - insert entity into table if necessary
     * - get new entity's ID
     * - insert event into table
     * - get proper oven dimensions (defaults if metallic oven is chosen)
     * - insert new OVEN into Ovens table
     * - send action to web server
     */

    qDebug() << "Registering carbonization beginning" << coordinate << timestamp
             << plotId << ovenId << userId << ovenType << ovenDimensions;

    // Check if Harvest ID already exists
    const QString harvestId(generateHarvestId(plotId, userId));

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));

    query.prepare("SELECT id, name FROM Entities WHERE name=:harvestId");
    query.bindValue(":harvestId", harvestId);

    if (query.exec() == false) {
        qWarning() << RED("Determining whether harves already exists has failed!")
                   << query.lastError().text() << "for query:" << query.lastQuery();
        return;
    }

    const bool alreadyPresent = query.next();

    // Insert a new Entity into table, if needed
    const QString typeId(findEntityTypeId(Enums::PackageType::Harvest));
    const QString parentEntityId(findEntityId(plotId));

    if (typeId.isEmpty()) {
        qWarning() << RED("Harvest ID type not found!");
        return;
    }

    if (alreadyPresent == false) {
        query.prepare("INSERT INTO Entities (typeId, name, parent, isFinished, isReplanted) "
                      "VALUES (:typeId, :harvestId, :parent, 0, 0)");
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
    }

    // Then, insert a new Event under that Entity
    const QString entityId(alreadyPresent? query.value("id").toString()
                                          : query.lastInsertId().toString());
    const QString eventTypeId(findEventTypeId(Enums::SupplyChainAction::CarbonizationBeginning));

    if (eventTypeId.isEmpty()) {
        qWarning() << RED("Event Type ID not found!");
        return;
    }

    query.prepare("INSERT INTO Events (entityId, typeId, userId, "
                  "date, locationLatitude, locationLongitude, isCommitted) "
                  "VALUES (:entityId, :typeId, :userId, :date, "
                  ":locationLatitude, :locationLongitude, 0)");
    query.bindValue(":entityId", entityId);
    query.bindValue(":typeId", eventTypeId);
    query.bindValue(":userId", userId);
    query.bindValue(":date", timestamp);
    query.bindValue(":locationLatitude", coordinate.latitude());
    query.bindValue(":locationLongitude", coordinate.longitude());

    if (query.exec() == false) {
        qWarning() << RED("Inserting Carbonization Beginning event has failed!")
                   << query.lastError().text() << "for query:" << query.lastQuery();
        return;
    }

    // Get proper oven dimensions
    QVariantList dimensions;
    if (ovenType == "metallic") {
        dimensions = defaultOvenDimensions(ovenType);
    } else {
        dimensions = ovenDimensions;
    }

    // Insert new oven into Ovens table
    const QString eventId(query.lastInsertId().toString());
    const QString ovenTypeId(findOvenTypeId(ovenType));

    query.prepare("INSERT INTO Ovens (type, plot, carbonizationBeginning, name, "
                  "height, length, width) "
                  "VALUES (:type, :plot, :event, :name, :height, :length, :width)");
    query.bindValue(":type", ovenTypeId);
    query.bindValue(":plot", parentEntityId);
    query.bindValue(":event", eventId);
    query.bindValue(":name", ovenId);
    query.bindValue(":height", dimensions.at(0));
    query.bindValue(":length", dimensions.at(1));
    query.bindValue(":width", dimensions.at(2));

    if (query.exec() == false) {
        qWarning() << RED("Inserting new oven has failed!")
                   << query.lastError().text() << "for query:" << query.lastQuery();
        return;
    }

    // Lastly, send a request to server to add it, too.
}

void ActionController::registerCarbonizationEnding(
    const QGeoCoordinate &coordinate, const QDateTime &timestamp,
    const QString &userId, const QString &harvestId, const QString &plotId,
    const QVariantList &ovenIds) const
{
    /*
     * Algorithm is:
     * - find entity ID in table (created in Carbonization Beginning step)
     * - insert separate ending event for each carbonization beginning into table
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

    for (const QVariant &idVar : ovenIds) {
        const QString ovenId(idVar.toString());
        QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));
        query.prepare("INSERT INTO Events (entityId, typeId, userId, "
                      "date, locationLatitude, locationLongitude, isCommitted) "
                      "VALUES (:entityId, :typeId, :userId, :date, "
                      ":locationLatitude, :locationLongitude, 0)");
        query.bindValue(":entityId", entityId);
        query.bindValue(":typeId", eventTypeId);
        query.bindValue(":userId", userId);
        query.bindValue(":date", timestamp);
        query.bindValue(":locationLatitude", coordinate.latitude());
        query.bindValue(":locationLongitude", coordinate.longitude());

        if (query.exec() == false) {
            qWarning() << RED("Inserting Logging Ending event has failed!")
                       << query.lastError().text() << "for query:" << query.lastQuery();
            return;
        }

        const QString eventId(query.lastInsertId().toString());

        query.prepare("UPDATE Ovens SET carbonizationEnding=:carbonizationEndingId "
                      "WHERE id=:ovenId");
        query.bindValue(":carbonizationEndingId", eventId);
        query.bindValue(":ovenId", ovenId);

        if (query.exec() == false) {
            qWarning() << RED("Updating oven with carbonization ending event ID has failed!")
                       << query.lastError().text() << "for query:" << query.lastQuery();
            return;
        }
    }

    // Lastly, send a request to server to add it, too.
}

void ActionController::registerTransportAndLoading(
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

    query.prepare("INSERT INTO Entities (typeId, name, parent, isFinished, isReplanted) "
                  "VALUES (:typeId, :transportId, :parent, 0, 0)");
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
                  "date, locationLatitude, locationLongitude, properties, "
                  "isCommitted) "
                  "VALUES (:entityId, :typeId, :userId, :date, "
                  ":locationLatitude, :locationLongitude, :properties, 0)");
    query.bindValue(":entityId", entityId);
    query.bindValue(":typeId", eventTypeId);
    query.bindValue(":userId", userId);
    query.bindValue(":date", timestamp);
    query.bindValue(":locationLatitude", coordinate.latitude());
    query.bindValue(":locationLongitude", coordinate.longitude());
    // TODO: use Tags to denote the properties more reliably!
    query.bindValue(":properties",
                    propertiesToString(QVariantMap {
                        { "harvestId", harvestId },
                        { "plateNumber", plateNumber },
                        { "destination", destination },
                        { "scannedQrs", scannedQrs }
                    }));

    if (query.exec() == false) {
        qWarning() << RED("Inserting Carbonization Beginning event has failed!")
                   << query.lastError().text() << "for query:" << query.lastQuery();
        return;
    }

    // Lastly, send a request to server to add it, too.
}

void ActionController::registerReception(
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
                  "date, locationLatitude, locationLongitude, properties, "
                  "isCommitted) "
                  "VALUES (:entityId, :typeId, :userId, :date, "
                  ":locationLatitude, :locationLongitude, :properties, 0)");
    query.bindValue(":entityId", entityId);
    query.bindValue(":typeId", eventTypeId);
    query.bindValue(":userId", userId);
    query.bindValue(":date", timestamp);
    query.bindValue(":locationLatitude", coordinate.latitude());
    query.bindValue(":locationLongitude", coordinate.longitude());
    // TODO: use Tags to denote the properties more reliably!
    query.bindValue(":properties",
                    propertiesToString(QVariantMap {
                        { "transportId", transportId },
                        { "documents", documents },
                        { "receipts", receipts },
                        { "scannedQrs", scannedQrs }
                    }));

    if (query.exec() == false) {
        qWarning() << RED("Inserting reception event has failed!")
                   << query.lastError().text() << "for query:" << query.lastQuery();
        return;
    }

    // Lastly, send a request to server to add it, too.
}

void ActionController::finalizeSupplyChain(const QString &plotId) const
{
    const QString parentId(findEntityId(plotId));
    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));
    query.prepare("UPDATE Entities SET isFinished=1 WHERE name=:plotId OR parent=:parentId");
    query.bindValue(":plotId", plotId);
    query.bindValue(":parentId", parentId);

    if (query.exec() == false) {
        qWarning() << RED("Finishing a supply chain has failed!")
                   << query.lastError().text()
                   << "for query:" << query.lastQuery()
                   << "with params:" << plotId << parentId;
        return;
    }
}

void ActionController::registerReplantation(
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
                  "beginningDate, endingDate, isCommitted) "
                  "VALUES (:plotId, :userId, "
                  ":numberOfTrees, :treeSpecies, "
                  ":locationLatitude, :locationLongitude, "
                  ":beginningDate, :endingDate, 0)");
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
QString ActionController::findEntityId(const QString &name) const
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

QString ActionController::findEntityTypeId(const Enums::PackageType type) const
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

QString ActionController::findEventTypeId(const Enums::SupplyChainAction action) const
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

QString ActionController::findTreeSpeciesId(const QString &species) const
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

QString ActionController::findOvenTypeId(const QString &ovenType) const
{
    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));

    query.prepare("SELECT id FROM OvenTypes WHERE name=:ovenType");
    query.bindValue(":ovenType", ovenType);

    if (query.exec()) {
        query.next();
        return query.value("id").toString();
    }

    qWarning() << RED("Getting OvenTypeId has failed!")
               << query.lastError().text()
               << "for query:" << query.lastQuery()
               << "DB:" << m_dbConnName;

    return QString();
}

QString ActionController::actionAbbreviation(const Enums::SupplyChainAction action) const
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

QString ActionController::propertiesToString(const QVariantMap &properties) const
{
    return QJsonDocument::fromVariant(properties).toJson(QJsonDocument::Compact);
}

int ActionController::scannedBagsForAction(const QString &transportId,
                                           const Enums::SupplyChainAction action) const
{
    const QString plotId(getPlotId(transportId));
    const QString entityTypeId(findEntityTypeId(Enums::PackageType::Transport));
    const QString eventTypeId(findEventTypeId(action));

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));

    query.prepare("SELECT properties FROM Events WHERE typeId=:eventTypeId "
                  "AND entityId IN (SELECT id FROM Entities WHERE parent=:plotId "
                  "AND typeId=:entityTypeId)");
    query.bindValue(":eventTypeId", eventTypeId);
    query.bindValue(":plotId", plotId);
    query.bindValue(":entityTypeId", entityTypeId);

    if (query.exec() == false) {
        qWarning() << RED("Getting total number of bags has failed!")
                   << query.lastError().text()
                   << "for query:" << query.lastQuery()
                   << "DB:" << m_dbConnName;
        return -1;
    }

    int total = 0;
    while(query.next()) {
        const QByteArray propertiesString(query.value("properties").toByteArray());
        const QJsonDocument propertiersJson(QJsonDocument::fromJson(propertiesString));
        const QVariantMap properties(propertiersJson.toVariant().toMap());
        total += properties.value("scannedQrs").toList().size();
    }

    return total;
}
