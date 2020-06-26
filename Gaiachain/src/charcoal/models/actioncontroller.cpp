#include "actioncontroller.h"

#include "database/dbhelpers.h"
#include "charcoal/database/charcoaldbhelpers.h"
#include "charcoal/picturesmanager.h"
#include "common/logs.h"
#include "common/tags.h"

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

void ActionController::setPicturesManager(PicturesManager *manager)
{
    m_picturesManager = manager;
}

QString ActionController::generatePlotId(const QString &userId,
                                         const QString &parcelCode,
                                         const QDate &date) const
{
    return userId + CharcoalDbHelpers::sep + parcelCode + CharcoalDbHelpers::sep
        + date.toString(dateFormat);
}

QString ActionController::generateHarvestId(const QString &plotId,
                                            const QString &userId) const
{
    return plotId + CharcoalDbHelpers::sep + userId;
}

QString ActionController::generateTransportId(const QString &harvestId,
                                              const QString &licensePlate,
                                              const int transportNumber,
                                              const QDate &date) const
{
    return harvestId + CharcoalDbHelpers::sep + licensePlate
        + CharcoalDbHelpers::sep + "T" + QString::number(transportNumber)
            + CharcoalDbHelpers::sep + date.toString(dateFormat);
}

QString ActionController::getPlotId(const QString &packageId)
{
    return CharcoalDbHelpers::getPlotId(packageId)   ;
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
            const QVariantList qrs(properties.value(Tags::webQrCodes).toList());

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
    const QString plotId(CharcoalDbHelpers::getPlotId(harvestId));
    const int parentEntityId(CharcoalDbHelpers::getEntityIdFromName(m_dbConnName, plotId));
    const int transportTypeId(CharcoalDbHelpers::getEntityTypeId(m_dbConnName, Enums::PackageType::Transport));

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
    const int transportEntityId(CharcoalDbHelpers::getEntityIdFromName(m_dbConnName, transportId));

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
        return properties.value(Tags::webQrCodes).toList().size();
    }

    qWarning() << RED("Getting bag count has failed!")
               << query.lastError().text()
               << "for query:" << query.lastQuery()
               << "DB:" << m_dbConnName;

    return -1;
}

QString ActionController::plateNumberInTransport(const QString &transportId) const
{
    const int transportEntityId(CharcoalDbHelpers::getEntityIdFromName(m_dbConnName, transportId));

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
        return properties.value(Tags::webPlateNumber).toString();
    }

    qWarning() << RED("Getting plate number has failed!")
               << query.lastError().text()
               << "for query:" << query.lastQuery()
               << "DB:" << m_dbConnName;

    return QString();
}

int ActionController::scannedBagsCount(const QString &transportId) const
{
    return scannedBagsForAction(transportId, Enums::SupplyChainAction::Reception);
}

int ActionController::scannedBagsTotal(const QString &transportId) const
{
    return scannedBagsForAction(transportId, Enums::SupplyChainAction::LoadingAndTransport);
}

int ActionController::registeredTrucksCount(const QString &transportId) const
{
    const int plotId(CharcoalDbHelpers::getEntityIdFromName(m_dbConnName, transportId));
    const int transportTypeId(CharcoalDbHelpers::getEntityTypeId(m_dbConnName, Enums::PackageType::Transport));
    const int receptionTypeId(CharcoalDbHelpers::getEventTypeId(
        m_dbConnName, Enums::SupplyChainAction::Reception));

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
    const QString plotId(CharcoalDbHelpers::getPlotId(transportId));
    const int parentEntityId(CharcoalDbHelpers::getEntityIdFromName(m_dbConnName, plotId));
    const int transportTypeId(CharcoalDbHelpers::getEntityTypeId(m_dbConnName, Enums::PackageType::Transport));

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
    const int parentEntityId(CharcoalDbHelpers::getEntityIdFromName(m_dbConnName, plotId));

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

QVariantList ActionController::defaultOvenDimensions(const int ovenType) const
{
    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));
    QVariantList dimensions;
    if (ovenType == 2) {
        query.prepare("SELECT oven_height, oven_width, oven_length FROM OvenTypes WHERE type=:ovenType");
        query.bindValue(":ovenType", ovenType);

        if (query.exec() == false) {
            qWarning() << RED("Getting metallic oven dimensions has failed!")
                       << query.lastError().text() << "for query:" << query.lastQuery();
            return QVariantList{ 0, 0, 0 };
        }

        query.next();
        dimensions.append(query.value("oven_height").toString());
        dimensions.append(query.value("oven_width").toString());
        dimensions.append(query.value("oven_length").toString());
    } else {
        return QVariantList{ 0, 0, 0 };
    }

    return dimensions;
}

void ActionController::registerLoggingBeginning(
    const QGeoCoordinate &coordinate,
    const QDateTime &timestamp, const QDateTime &eventDate,
    const QString &userId,
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
    const int typeId(CharcoalDbHelpers::getEntityTypeId(m_dbConnName, Enums::PackageType::Plot));

    if (typeId == -1) {
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
    const int eventTypeId(CharcoalDbHelpers::getEventTypeId(
        m_dbConnName, Enums::SupplyChainAction::LoggingBeginning));
    const int villageId(CharcoalDbHelpers::getVillageId(m_dbConnName, village));
    const int parcelId(CharcoalDbHelpers::getParcelId(m_dbConnName, parcel));
    const int treeSpeciesId(CharcoalDbHelpers::getTreeSpeciesId(m_dbConnName, treeSpecies));

    if (eventTypeId == -1) {
        qWarning() << RED("Event Type ID not found!");
        return;
    }

    query.prepare("INSERT INTO Events (entityId, typeId, userId,"
                  "date, eventDate, locationLatitude, locationLongitude, properties, "
                  "isCommitted) "
                  "VALUES (:entityId, :typeId, :userId, :date, :eventDate, "
                  ":locationLatitude, :locationLongitude, :properties, 0)");
    query.bindValue(":entityId", entityId);
    query.bindValue(":typeId", eventTypeId);
    query.bindValue(":userId", userId);
    query.bindValue(":date", timestamp.toSecsSinceEpoch());
    query.bindValue(":eventDate", eventDate.toSecsSinceEpoch());
    query.bindValue(":locationLatitude", coordinate.latitude());
    query.bindValue(":locationLongitude", coordinate.longitude());
    // TODO: use Tags to denote the properties more reliably!
    query.bindValue(":properties",
                    propertiesToString(QVariantMap {
                        { "parcel", parcelId },
                        { "village", villageId },
                        { Tags::webTreeSpecies, treeSpeciesId },
                        { Tags::webEventDate, eventDate.toSecsSinceEpoch() },
                    }));

    if (query.exec() == false) {
        qWarning() << RED("Inserting Logging Beginning event has failed!")
                   << query.lastError().text() << "for query:" << query.lastQuery();
        return;
    }

    // Update Parcels entry
    query.prepare("UPDATE Parcels SET isUsed=1 WHERE id=:parcelId");
    query.bindValue(":parcelId", parcelId);

    if (query.exec() == false) {
        qWarning() << RED("Updating Parcel status has failed!")
                   << query.lastError().text() << "for query:" << query.lastQuery()
                   << parcelId;
        return;
    }

    emit refreshLocalEvents();
}

void ActionController::registerLoggingEnding(
    const QGeoCoordinate &coordinate,
    const QDateTime &timestamp, const QDateTime &eventDate,
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

    const int entityId(CharcoalDbHelpers::getEntityIdFromName(m_dbConnName, plotId));

    if (entityId == -1) {
        qWarning() << RED("Entity ID not found!");
        return;
    }

    const int eventTypeId(CharcoalDbHelpers::getEventTypeId(
        m_dbConnName, Enums::SupplyChainAction::LoggingEnding));

    if (eventTypeId == -1) {
        qWarning() << RED("Event Type ID not found!");
        return;
    }

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));
    query.prepare("INSERT INTO Events (entityId, typeId, userId,"
                  "date, eventDate, locationLatitude, locationLongitude, properties, "
                  "isCommitted) "
                  "VALUES (:entityId, :typeId, :userId, :date, :eventDate, "
                  ":locationLatitude, :locationLongitude, :properties, 0)");
    query.bindValue(":entityId", entityId);
    query.bindValue(":typeId", eventTypeId);
    query.bindValue(":userId", userId);
    query.bindValue(":date", timestamp.toSecsSinceEpoch());
    query.bindValue(":eventDate", eventDate.toSecsSinceEpoch());
    query.bindValue(":locationLatitude", coordinate.latitude());
    query.bindValue(":locationLongitude", coordinate.longitude());
    // TODO: use Tags to denote the properties more reliably!
    query.bindValue(":properties",
                    propertiesToString(QVariantMap {
                        { Tags::webNumberOfTrees, numberOfTrees },
                        { Tags::webEventDate, eventDate.toSecsSinceEpoch() }
                    }));

    if (query.exec() == false) {
        qWarning() << RED("Inserting Logging Ending event has failed!")
                   << query.lastError().text() << "for query:" << query.lastQuery();
        return;
    }

    emit refreshLocalEvents();
}

void ActionController::registerCarbonizationBeginning(
    const QGeoCoordinate &coordinate,
    const QDateTime &timestamp, const QDateTime &eventDate,
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
    const int typeId(CharcoalDbHelpers::getEntityTypeId(m_dbConnName, Enums::PackageType::Harvest));
    const int parentEntityId(CharcoalDbHelpers::getEntityIdFromName(m_dbConnName, plotId));

    if (typeId == -1) {
        qWarning() << RED("Harvest ID type not found!");
        return;
    }

    if (alreadyPresent == false) {
        query.prepare("INSERT INTO Entities (typeId, name, parent, "
                      "isFinished, isReplanted) "
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
    const int eventTypeId(CharcoalDbHelpers::getEventTypeId(
        m_dbConnName, Enums::SupplyChainAction::CarbonizationBeginning));
    const int webPlotId(
        CharcoalDbHelpers::getWebPackageId(m_dbConnName, parentEntityId));

    if (eventTypeId == -1) {
        qWarning() << RED("Event Type ID not found!");
        return;
    }
    query.prepare("INSERT INTO Events (entityId, typeId, userId, "
                  "date, eventDate, locationLatitude, locationLongitude, "
                  "properties, isCommitted) "
                  "VALUES (:entityId, :typeId, :userId, :date, :eventDate, "
                  ":locationLatitude, :locationLongitude, :properties, 0)");
    query.bindValue(":entityId", entityId);
    query.bindValue(":typeId", eventTypeId);
    query.bindValue(":userId", userId);
    query.bindValue(":date", timestamp.toSecsSinceEpoch());
    query.bindValue(":eventDate", eventDate.toSecsSinceEpoch());
    query.bindValue(":locationLatitude", coordinate.latitude());
    query.bindValue(":locationLongitude", coordinate.longitude());
    // TODO: use Tags to denote the properties more reliably!
    query.bindValue(":properties",
                    propertiesToString(QVariantMap {
                        { Tags::webOvenType, ovenType },
                        { Tags::webEventDate, eventDate.toSecsSinceEpoch() },
                        { Tags::webPlotId, webPlotId },
                        { Tags::webOvenId, ovenId }
                    }));

    if (query.exec() == false) {
        qWarning() << RED("Inserting Carbonization Beginning event has failed!")
                   << query.lastError().text() << "for query:" << query.lastQuery();
        return;
    }

    // Get proper oven dimensions
    QVariantList dimensions;
    const int ovenTypeInt = ovenType.toInt();
    if (ovenTypeInt == 2) {
        dimensions = defaultOvenDimensions(ovenTypeInt);
    } else {
        dimensions = ovenDimensions;
    }

    // Insert new oven into Ovens table
    const QString eventId(query.lastInsertId().toString());
    const QString ovenTypeId(findOvenTypeId(ovenType));

    query.prepare("INSERT INTO Ovens (type, plot, carbonizationBeginning, name, "
                  "oven_height, oven_width, oven_length) "
                  "VALUES (:type, :plot, :event, :name, :height, :width, :length)");
    query.bindValue(":type", ovenTypeId);
    query.bindValue(":plot", parentEntityId);
    query.bindValue(":event", eventId);
    query.bindValue(":name", ovenId);
    // Height, length, width is the order from GUI
    query.bindValue(":height", dimensions.at(0));
    query.bindValue(":length", dimensions.at(1));
    query.bindValue(":width", dimensions.at(2));

    if (query.exec() == false) {
        qWarning() << RED("Inserting new oven has failed!")
                   << query.lastError().text() << "for query:" << query.lastQuery();
        return;
    }

    emit refreshLocalEvents();
}

void ActionController::registerCarbonizationEnding(
    const QGeoCoordinate &coordinate,
    const QDateTime &timestamp, const QDateTime &eventDate,
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

    const int entityId(CharcoalDbHelpers::getEntityIdFromName(m_dbConnName, harvestId));

    if (entityId == -1) {
        qWarning() << RED("Entity ID not found!");
        return;
    }

    const int eventTypeId(CharcoalDbHelpers::getEventTypeId(m_dbConnName, Enums::SupplyChainAction::CarbonizationEnding));

    if (eventTypeId == -1) {
        qWarning() << RED("Event Type ID not found!");
        return;
    }

    for (const QVariant &idVar : ovenIds) {
        const int ovenId(idVar.toInt());
        const QString ovenLetter(CharcoalDbHelpers::getOvenLetter(m_dbConnName, ovenId));

        QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));
        query.prepare("INSERT INTO Events (entityId, typeId, userId, "
                      "date, eventDate, locationLatitude, locationLongitude, "
                      "properties, isCommitted) "
                      "VALUES (:entityId, :typeId, :userId, :date, :eventDate, "
                      ":locationLatitude, :locationLongitude, :properties, 0)");
        query.bindValue(":entityId", entityId);
        query.bindValue(":typeId", eventTypeId);
        query.bindValue(":userId", userId);
        query.bindValue(":date", timestamp.toSecsSinceEpoch());
        query.bindValue(":eventDate", eventDate.toSecsSinceEpoch());
        query.bindValue(":locationLatitude", coordinate.latitude());
        query.bindValue(":locationLongitude", coordinate.longitude());
        // TODO: use Tags to denote the properties more reliably!
        query.bindValue(":properties",
                        propertiesToString(QVariantMap {
                            { Tags::webOvenId, ovenLetter },
                            { Tags::webEventDate, eventDate.toSecsSinceEpoch() }
                        }));

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

    emit refreshLocalEvents();
}

void ActionController::registerLoadingAndTransport(
    const QGeoCoordinate &coordinate,
    const QDateTime &timestamp, const QDateTime &eventDate,
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
    const int typeId(CharcoalDbHelpers::getEntityTypeId(m_dbConnName, Enums::PackageType::Transport));
    const int parentEntityId(CharcoalDbHelpers::getEntityIdFromName(
        m_dbConnName, CharcoalDbHelpers::getPlotId(transportId)));
    const int harvestEntity(CharcoalDbHelpers::getEntityIdFromName(m_dbConnName, harvestId));
    const int webHarvestId(
        CharcoalDbHelpers::getWebPackageId(m_dbConnName, harvestEntity));

    if (typeId == -1) {
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
    const int eventTypeId(CharcoalDbHelpers::getEventTypeId(m_dbConnName, Enums::SupplyChainAction::LoadingAndTransport));
    const int destinationId(CharcoalDbHelpers::getDestinationId(m_dbConnName, destination));

    if (eventTypeId == -1) {
        qWarning() << RED("Event Type ID not found!");
        return;
    }

    query.prepare("INSERT INTO Events (entityId, typeId, userId,"
                  "date, eventDate, locationLatitude, locationLongitude, properties, "
                  "isCommitted) "
                  "VALUES (:entityId, :typeId, :userId, :date, :eventDate, "
                  ":locationLatitude, :locationLongitude, :properties, 0)");
    query.bindValue(":entityId", entityId);
    query.bindValue(":typeId", eventTypeId);
    query.bindValue(":userId", userId);
    query.bindValue(":date", timestamp.toSecsSinceEpoch());
    query.bindValue(":eventDate", eventDate.toSecsSinceEpoch());
    query.bindValue(":locationLatitude", coordinate.latitude());
    query.bindValue(":locationLongitude", coordinate.longitude());

    query.bindValue(":properties",
                    propertiesToString(QVariantMap {
                        { Tags::webHarvestId, webHarvestId },
                        { Tags::webPlateNumber, plateNumber },
                        { "destination", destinationId },
                        { Tags::webQrCodes, scannedQrs },
                        { Tags::webEventDate, eventDate.toSecsSinceEpoch() }
                    }));

    if (query.exec() == false) {
        qWarning() << RED("Inserting Carbonization Beginning event has failed!")
                   << query.lastError().text() << "for query:" << query.lastQuery();
        return;
    }

    emit refreshLocalEvents();
}

void ActionController::registerReception(
    const QGeoCoordinate &coordinate,
    const QDateTime &timestamp, const QDateTime &eventDate,
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

    const int entityId(CharcoalDbHelpers::getEntityIdFromName(m_dbConnName, transportId));

    if (entityId == -1) {
        qWarning() << RED("Entity ID not found!");
        return;
    }

    const int eventTypeId(CharcoalDbHelpers::getEventTypeId(m_dbConnName, Enums::SupplyChainAction::Reception));

    if (eventTypeId == -1) {
        qWarning() << RED("Event Type ID not found!");
        return;
    }

    const QStringList cachedDocs(m_picturesManager->moveToCache(documents));
    const QStringList cachedRecs(m_picturesManager->moveToCache(receipts));

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));
    query.prepare("INSERT INTO Events (entityId, typeId, userId,"
                  "date, eventDate, locationLatitude, locationLongitude, properties, "
                  "isCommitted) "
                  "VALUES (:entityId, :typeId, :userId, :date, :eventDate, "
                  ":locationLatitude, :locationLongitude, :properties, 0)");
    query.bindValue(":entityId", entityId);
    query.bindValue(":typeId", eventTypeId);
    query.bindValue(":userId", userId);
    query.bindValue(":date", timestamp.toSecsSinceEpoch());
    query.bindValue(":eventDate", eventDate.toSecsSinceEpoch());
    query.bindValue(":locationLatitude", coordinate.latitude());
    query.bindValue(":locationLongitude", coordinate.longitude());

    query.bindValue(":properties",
                    propertiesToString(QVariantMap {
                        //{ "transportId", transportId },
                        { Tags::documents, cachedDocs },
                        { Tags::receipts, cachedRecs },
                        { Tags::webQrCodes, scannedQrs },
                        { Tags::webEventDate, eventDate.toSecsSinceEpoch() }
                    }));

    if (query.exec() == false) {
        qWarning() << RED("Inserting reception event has failed!")
                   << query.lastError().text() << "for query:" << query.lastQuery();
        return;
    }

    emit refreshLocalEvents();
}

void ActionController::finalizeSupplyChain(const QString &plotId) const
{
    const int parentId(CharcoalDbHelpers::getEntityIdFromName(m_dbConnName, plotId));
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
    const QGeoCoordinate &coordinate,
    const QDateTime &timestamp,
    const QString &userId, const QString &plotId,
    const int numberOfTrees, const QString &treeSpecies,
    const QDateTime &beginningDate, const QDateTime &endingDate) const
{
    /*
     * Algorithm is:
     * - insert replantation entry into table
     * - mark parent entity (plot) as replanted (set isReplanted to 1)
     * - send action to web server
     */

    qDebug() << "Registering replantation" << coordinate << timestamp
             << userId << plotId << numberOfTrees << treeSpecies
             << beginningDate << endingDate;

    const int parentId(CharcoalDbHelpers::getEntityIdFromName(m_dbConnName, plotId));
    const int treeSpeciesId(CharcoalDbHelpers::getTreeSpeciesId(m_dbConnName, treeSpecies));

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));

    query.prepare("INSERT INTO Replantations (plotId, userId, "
                  "date, "
                  "numberOfTrees, treeSpecies, "
                  "locationLatitude, locationLongitude, "
                  "beginningDate, endingDate, isCommitted) "
                  "VALUES (:plotId, :userId, "
                  ":date, "
                  ":numberOfTrees, :treeSpecies, "
                  ":locationLatitude, :locationLongitude, "
                  ":beginningDate, :endingDate, 0)");
    query.bindValue(":plotId", parentId);
    query.bindValue(":userId", userId);
    query.bindValue(":date", timestamp.toSecsSinceEpoch());
    query.bindValue(":numberOfTrees", numberOfTrees);
    query.bindValue(":treeSpecies", treeSpeciesId);
    query.bindValue(":locationLatitude", coordinate.latitude());
    query.bindValue(":locationLongitude", coordinate.longitude());
    query.bindValue(":beginningDate", beginningDate.toSecsSinceEpoch());
    query.bindValue(":endingDate", endingDate.toSecsSinceEpoch());

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

    emit refreshLocalEvents();
}

QString ActionController::findOvenTypeId(const QString &ovenType) const
{
    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));

    query.prepare("SELECT id FROM OvenTypes WHERE type=:ovenType");
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

QString ActionController::propertiesToString(const QVariantMap &properties) const
{
    return QJsonDocument::fromVariant(properties).toJson(QJsonDocument::Compact);
}

int ActionController::scannedBagsForAction(const QString &transportId,
                                           const Enums::SupplyChainAction action) const
{
    const int plotId(CharcoalDbHelpers::getEntityIdFromName(m_dbConnName, transportId));
    const int entityTypeId(CharcoalDbHelpers::getEntityTypeId(m_dbConnName, Enums::PackageType::Transport));
    const int eventTypeId(CharcoalDbHelpers::getEventTypeId(m_dbConnName, action));

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
        total += properties.value(Tags::webQrCodes).toList().size();
    }

    return total;
}
