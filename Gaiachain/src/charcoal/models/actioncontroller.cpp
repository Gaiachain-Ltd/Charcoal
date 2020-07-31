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
    qRegisterMetaType<BagsMatch>("BagsMatch");
}

void ActionController::setDbConnection(const QString &connectionName)
{
    m_connectionName = connectionName;
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

/*!
 * Returns true if plot generated from \a userId, \a parcelCode and \a date is
 * already present.
 */
bool ActionController::plotExists(const QString &userId,
                                  const QString &parcelCode,
                                  const QDate &date)
{
    const QString plotName(generatePlotId(userId, parcelCode, date));
    const int id = CharcoalDbHelpers::getEntityIdFromName(
        m_connectionName, plotName);
    return (id > 0);
}

QString ActionController::getPlotId(const QString &packageId)
{
    return CharcoalDbHelpers::getPlotName(packageId);
}

int ActionController::getTransportIdFromBags(const QVariantList &scannedQrs) const
{
    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));

    query.prepare("SELECT properties, entityId FROM Events "
                  "WHERE isPaused=0 "
                  "AND entityId IN (SELECT id FROM Entities WHERE isFinished=0) "
                  "AND typeId IN (SELECT id FROM EventTypes WHERE actionName=\"TR\") "
                  "ORDER BY entityId DESC");

    int transportEntityId = -1;
    if (query.exec()) {
        while (query.next()) {
            const QByteArray propertiesString(query.value(Tags::properties).toByteArray());
            const QJsonObject properties(
                CharcoalDbHelpers::dbPropertiesToJson(propertiesString));
            const QVariantList qrs(properties.value(Tags::webQrCodes).toArray().toVariantList());

            bool found = false;
            for (const QVariant &qr : scannedQrs) {
                if (qrs.contains(qr)) {
                    bool ok = true;
                    const int result = query.value(Tags::entityId).toInt(&ok);

                    if (ok) {
                        transportEntityId = result;
                        found = true;
                        break;
                    }

                    //qDebug() << "HIT!" << qr.toString() << transportEntityId;
                }
            }

            if (found) {
                break;
            }
        }
    } else {
        qWarning() << RED("Getting transport ID from bags has failed!")
                   << query.lastError().text()
                   << "for query:" << query.lastQuery()
                   << "DB:" << m_connectionName;
    }

    if (transportEntityId == -1) {
        const QString msg(tr("No active transport matching scanned bags has been found!"));
        qWarning() << RED(msg) << scannedQrs;
        emit error(msg);
    }

    return transportEntityId;
}

QString ActionController::getEntityName(const int id) const
{
    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));
    query.prepare("SELECT name FROM Entities WHERE id=:id");
    query.bindValue(":id", id);

    if (query.exec()) {
        query.next();
        return query.value(Tags::name).toString();
    }

    qWarning() << RED("Getting Entity name has failed!")
               << query.lastError().text()
               << "for query:" << query.lastQuery()
               << "DB:" << m_connectionName;

    return QString();
}

int ActionController::nextTransportNumber(const int harvestId, const bool isPausedEvent) const
{
    const int parentEntityId(CharcoalDbHelpers::getParentEntityId(
        m_connectionName, harvestId));
    const int transportTypeId(CharcoalDbHelpers::getEntityTypeId(
        m_connectionName, Enums::PackageType::Transport));

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));

    query.prepare("SELECT id FROM Entities WHERE parent=:parentEntityId "
                  "AND typeId=:transportTypeId ");
    query.bindValue(":parentEntityId", parentEntityId);
    query.bindValue(":transportTypeId", transportTypeId);

    if (query.exec()) {
        // We can't use query.size() because SQLITE does not support it
        int size = 0;
        while (query.next()) {
            size++;
        }

        if (isPausedEvent == false) {
            size++;
        }

        return size;
    }

    qWarning() << RED("Getting next transport number has failed!")
               << query.lastError().text()
               << "for query:" << query.lastQuery()
               << "DB:" << m_connectionName;
    return -1;
}

int ActionController::bagCountInTransport(const int transportId) const
{
    return CharcoalDbHelpers::bagCountInTransport(m_connectionName, transportId);
}

BagsMatch ActionController::matchBags(const int transportId,
                                 QVariantList qrsFromReception)
{
    BagsMatch result;

    const int typeId = CharcoalDbHelpers::getEventTypeId(
        m_connectionName, Enums::SupplyChainAction::LoadingAndTransport);

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));

    query.prepare("SELECT properties FROM Events "
                  "WHERE entityId=:transportId AND typeId=:typeId");
    query.bindValue(":transportId", transportId);
    query.bindValue(":typeId", typeId);

    if (query.exec() == false || query.next() == false) {
        const QString msg(tr("Could not check bags for transport %1").arg(transportId));
        qWarning() << msg;
        emit error(msg);
        return result;
    }

    const QByteArray propertiesString(query.value(Tags::properties).toByteArray());
    const QJsonObject properties(
        CharcoalDbHelpers::dbPropertiesToJson(propertiesString));
    QVariantList qrsFromTransport(properties.value(Tags::webQrCodes).toArray().toVariantList());

    if (qrsFromReception.size() != qrsFromTransport.size()) {
        const QString msg(tr("The number of scanned QR codes (%1) does not "
                             "match the number of bags sent in transport (%2)")
                              .arg(qrsFromTransport.size()).arg(qrsFromReception.size()));
        qWarning() << msg;
        emit error(msg);
        return result;
    }

    std::sort(qrsFromReception.begin(), qrsFromReception.end());
    std::sort(qrsFromTransport.begin(), qrsFromTransport.end());

    if (qrsFromReception == qrsFromTransport) {
        result.fullMatch = true;
        return result;
    }

    for (const QVariant &transport : qAsConst(qrsFromTransport)) {
        if (qrsFromReception.contains(transport) == false) {
            result.missingBags.append(transport);
        }
    }

    for (const QVariant &reception : qAsConst(qrsFromReception)) {
        if (qrsFromTransport.contains(reception) == false) {
            result.extraBags.append(reception);
        }
    }

    if (result.fullMatch == false) {
        const QString msg(tr("Scanned QR codes do not match bags sent in transport"));
        qWarning() << msg;
        emit error(msg);
    }

    return result;
}

QString ActionController::plateNumberInTransport(const int transportId) const
{
    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));

    query.prepare("SELECT properties FROM Events WHERE entityId=:transportEntityId");
    query.bindValue(":transportEntityId", transportId);

    if (query.exec()) {
        query.next();
        const QByteArray propertiesString(query.value(Tags::properties).toByteArray());
        const QJsonObject properties(
            CharcoalDbHelpers::dbPropertiesToJson(propertiesString));
        return properties.value(Tags::webPlateNumber).toString();
    }

    qWarning() << RED("Getting plate number has failed!")
               << query.lastError().text()
               << "for query:" << query.lastQuery()
               << "DB:" << m_connectionName;

    return QString();
}

int ActionController::scannedBagsCount(const int transportId) const
{
    return scannedBagsForAction(transportId, Enums::SupplyChainAction::Reception);
}

int ActionController::scannedBagsTotal(const int transportId) const
{
    return scannedBagsForAction(transportId, Enums::SupplyChainAction::LoadingAndTransport);
}

int ActionController::registeredTrucksCount(const int transportId) const
{
    const int receptionTypeId(CharcoalDbHelpers::getEventTypeId(
        m_connectionName, Enums::SupplyChainAction::Reception));

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));

    // It's a bit "hacky" we count only Reception events, that gives us number
    // of events where transport has left but not arrived yet.
    query.prepare("SELECT COUNT(id) FROM Events WHERE typeId=:receptionTypeId "
                  "AND entityId=:transportId");
    query.bindValue(":receptionTypeId", receptionTypeId);
    query.bindValue(":transportId", transportId);

    if (query.exec() == false) {
        qWarning() << RED("Getting number of received trucks has failed!")
                   << query.lastError().text()
                   << "for query:" << query.lastQuery()
                   << "DB:" << m_connectionName;
        return -1;
    }

    query.next();
    return query.value(0).toInt();
}

int ActionController::registeredTrucksTotal(const int transportId) const
{
    const int plotId(CharcoalDbHelpers::getParentEntityId(
        m_connectionName, transportId));
    const int transportTypeId(CharcoalDbHelpers::getEntityTypeId(
        m_connectionName, Enums::PackageType::Transport));

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));

    query.prepare("SELECT COUNT(id) FROM Entities WHERE parent=:parentEntityId "
                  "AND typeId=:transportTypeId");
    query.bindValue(":parentEntityId", plotId);
    query.bindValue(":transportTypeId", transportTypeId);

    if (query.exec() == false) {
        qWarning() << RED("Getting total number of trucks has failed!")
                   << query.lastError().text()
                   << "for query:" << query.lastQuery()
                   << "DB:" << m_connectionName;
        return -1;
    }

    query.next();
    return query.value(0).toInt();
}

QString ActionController::nextOvenNumber(int parentEntityId) const
{
    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));

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
            emit error(tr("Too many ovens (%1) are assotiated with this harvest!")
                       .arg(size - 1));
            return QString();
        }

        //qDebug() << "Next oven for" << parentEntityId << "is" << QChar(size);
        return QChar(size);
    }

    qWarning() << RED("Getting next oven letter has failed!")
               << query.lastError().text()
               << "for query:" << query.lastQuery()
               << "DB:" << m_connectionName;

    emit error(tr("Could not determine next oven letter for given harvest"));
    return QString();
}

OvenDimensions ActionController::defaultOvenDimensions(const int ovenId) const
{
    return CharcoalDbHelpers::defaultOvenDimensions(m_connectionName, ovenId);
}

OvenDimensions ActionController::emptyOvenDimensions() const
{
    OvenDimensions result;
    return result;
}

qreal ActionController::ovenVolume(const qreal width, const qreal height,
                                   const qreal length) const
{
    return CharcoalDbHelpers::ovenVolume(width, length, height);
}

qreal ActionController::ovenVolume(const qreal width,
                                   const qreal height1,
                                   const qreal height2,
                                   const qreal length) const
{
    return CharcoalDbHelpers::ovenVolume(width, length, height1, height2);
}

void ActionController::registerLoggingBeginning(
    const QGeoCoordinate &coordinate,
    const QDateTime &timestamp,
    const QDateTime &eventDate,
    const QString &userId,
    const QString &parcel,
    const int parcelId,
    const int villageId,
    const int treeSpeciesId) const
{
    /*
     * Algorithm is:
     * - insert entity into table
     * - get new entity's ID
     * - insert event into table
     * - send action to web server
     */

    qDebug() << "Registering logging beginning" << coordinate << timestamp
             << parcel << userId << villageId << treeSpeciesId;

    // First, insert a new Entity into table
    const QString plotId(generatePlotId(userId, parcel, eventDate.date()));
    const int typeId(CharcoalDbHelpers::getEntityTypeId(
        m_connectionName, Enums::PackageType::Plot));

    if (typeId == -1) {
        qWarning() << RED("Plot ID type not found!");
        return;
    }

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));

    if (insertEntity(&query, typeId, plotId, -1) == false) {
        return;
    }

    // Then, insert a new Event under that Entity
    const int entityId(query.lastInsertId().toInt());
    const int eventTypeId(CharcoalDbHelpers::getEventTypeId(
        m_connectionName, Enums::SupplyChainAction::LoggingBeginning));

    if (eventTypeId == -1) {
        qWarning() << RED("Event Type ID not found!");
        return;
    }

    if (false == insertEvent(&query, entityId, eventTypeId, userId, timestamp,
                             eventDate, coordinate,
                             {
                                 { Tags::webParcel, parcelId },
                                 { Tags::webVillage, villageId },
                                 { Tags::webTreeSpecies, treeSpeciesId },
                                 { Tags::webEventDate, eventDate.toSecsSinceEpoch() }
                             }))
    {
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
    const QDateTime &timestamp,
    const QDateTime &eventDate,
    const QString &userId,
    const int plotId,
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

    if (plotId == -1) {
        qWarning() << RED("Entity ID not found!");
        return;
    }

    const int eventTypeId(CharcoalDbHelpers::getEventTypeId(
        m_connectionName, Enums::SupplyChainAction::LoggingEnding));

    if (eventTypeId == -1) {
        qWarning() << RED("Event Type ID not found!");
        return;
    }

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));

    if (false == insertEvent(&query, plotId, eventTypeId, userId, timestamp,
                             eventDate, coordinate,
                             {
                                 { Tags::webNumberOfTrees, numberOfTrees },
                                 { Tags::webEventDate, eventDate.toSecsSinceEpoch() }
                             }))
    {
        return;
    }

    emit refreshLocalEvents();
}

void ActionController::registerCarbonizationBeginning(
    const QGeoCoordinate &coordinate,
    const QDateTime &timestamp,
    const QDateTime &eventDate,
    const QString &userId,
    const QString &plotId,
    const int plotDbId,
    const QString &ovenId,
    const int ovenIdNumber,
    const OvenDimensions &ovenDimensions) const
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
             << plotId << plotDbId << ovenId << userId << ovenIdNumber
             << ovenDimensions;

    const int parentEntityId(plotDbId);

    // Check if Harvest ID already exists
    const QString harvestId(generateHarvestId(plotId, userId));

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));

    query.prepare("SELECT id, name FROM Entities "
                  "WHERE name=:harvestId AND parent=:parentEntityId");
    query.bindValue(":harvestId", harvestId);
    query.bindValue(":parentEntityId", parentEntityId);

    if (query.exec() == false) {
        qWarning() << RED("Determining whether harvest already exists has failed!")
                   << query.lastError().text() << "for query:" << query.lastQuery();
        return;
    }

    const bool alreadyPresent = query.next();

    if (alreadyPresent == false) {
        // Insert a new Entity into table, if needed
        const int typeId(CharcoalDbHelpers::getEntityTypeId(
            m_connectionName, Enums::PackageType::Harvest));

        if (typeId == -1) {
            qWarning() << RED("Harvest ID type not found!");
            return;
        }

        if (insertEntity(&query, typeId, harvestId, parentEntityId) == false) {
            return;
        }
    }

    // Then, insert a new Event under that Entity
    const int entityId(alreadyPresent? query.value(Tags::id).toInt()
                                      : query.lastInsertId().toInt());
    query.finish();

    const int eventTypeId(CharcoalDbHelpers::getEventTypeId(
        m_connectionName, Enums::SupplyChainAction::CarbonizationBeginning));

    if (eventTypeId == -1) {
        qWarning() << RED("Event Type ID not found!");
        return;
    }

    // Get proper oven dimensions
    const Enums::OvenType ovenType = CharcoalDbHelpers::getOvenType(
        m_connectionName, ovenIdNumber);
    OvenDimensions dimensions;
    if (ovenType == Enums::OvenType::Metallic) {
        dimensions = defaultOvenDimensions(ovenIdNumber);
    } else {
        dimensions = ovenDimensions;
    }

    QVariantMap properties {
        { Tags::webOvenType, ovenIdNumber },
        { Tags::webEventDate, eventDate.toSecsSinceEpoch() },
        { Tags::webOvenId, ovenId }
    };

    if (ovenType != Enums::OvenType::Metallic) {
        properties.insert(Tags::webOvenHeight, dimensions.height1);
        properties.insert(Tags::webOvenHeight2, dimensions.height2);
        properties.insert(Tags::webOvenLength, dimensions.length);
        properties.insert(Tags::webOvenWidth, dimensions.width);
        properties.insert(Tags::webOvenVolume, dimensions.volume());
    }

    if (false == insertEvent(&query, entityId, eventTypeId, userId, timestamp,
                             eventDate, coordinate, properties))
    {
        return;
    }

    // Insert new oven into Ovens table
    const QString eventId(query.lastInsertId().toString());

    query.prepare("INSERT INTO Ovens (type, plot, carbonizationBeginning, name, "
                  "oven_height, oven_height2, oven_width, oven_length) "
                  "VALUES (:type, :plot, :event, :name, "
                  ":height, :height2, :width, :length)");
    query.bindValue(":type", ovenIdNumber);
    query.bindValue(":plot", parentEntityId);
    query.bindValue(":event", eventId);
    query.bindValue(":name", ovenId);
    query.bindValue(":height", dimensions.height1);
    query.bindValue(":height2", dimensions.height2);
    query.bindValue(":length", dimensions.length);
    query.bindValue(":width", dimensions.width);

    if (query.exec() == false) {
        qWarning() << RED("Inserting new oven has failed!")
                   << query.lastError().text() << "for query:" << query.lastQuery();
        return;
    }

    emit refreshLocalEvents();
}

void ActionController::registerCarbonizationEnding(
    const QGeoCoordinate &coordinate,
    const QDateTime &timestamp,
    const QDateTime &eventDate,
    const QString &userId,
    const int harvestId,
    const QVariantList &ovenIds) const
{
    /*
     * Algorithm is:
     * - find entity ID in table (created in Carbonization Beginning step)
     * - insert separate ending event for each carbonization beginning into table
     * - send action to web server
     */

    qDebug() << "Registering carbonization ending" << coordinate << timestamp
             << harvestId << ovenIds << userId;

    if (harvestId == -1) {
        qWarning() << RED("Entity ID not found!");
        return;
    }

    const int eventTypeId(CharcoalDbHelpers::getEventTypeId(
        m_connectionName, Enums::SupplyChainAction::CarbonizationEnding));

    if (eventTypeId == -1) {
        qWarning() << RED("Event Type ID not found!");
        return;
    }

    const int count = ovenIds.count();
    quint64 uniqueTimestamp = timestamp.toSecsSinceEpoch() - count;

    for (const QVariant &idVar : ovenIds) {
        const int ovenId(idVar.toInt());
        const QString ovenLetter(CharcoalDbHelpers::getOvenLetter(m_connectionName, ovenId));

        QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));

        if (false == insertEvent(&query, harvestId, eventTypeId, userId,
                                 QDateTime::fromSecsSinceEpoch(uniqueTimestamp),
                                 eventDate, coordinate,
                                 {
                                     { Tags::webOvenId, ovenLetter },
                                     { Tags::webEventDate, eventDate.toSecsSinceEpoch() }
                                 }))
        {
            return;
        }

        uniqueTimestamp++;

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
    const QDateTime &timestamp,
    const QDateTime &eventDate,
    const QString &userId,
    const QString &transportId,
    const int harvestId,
    const QString &plateNumber,
    const QString &destination,
    const int destinationId,
    const QVariantList &scannedQrs,
    const bool pauseEvent) const
{
    /*
     * Algorithm is:
     * - insert entity into table
     * - get new entity's ID
     * - insert event into table
     * - send action to web server
     */

    qDebug() << "Registering transport and loading" << coordinate << timestamp
             << userId << transportId << plateNumber
             << destination << destinationId << scannedQrs.size()
             << pauseEvent;

    // First, insert a new Entity into table
    const int typeId(CharcoalDbHelpers::getEntityTypeId(
        m_connectionName, Enums::PackageType::Transport));

    const int parentEntityId(CharcoalDbHelpers::getParentEntityId(
        m_connectionName, harvestId));

    if (typeId == -1) {
        qWarning() << RED("Transport ID type not found!");
        return;
    }

    const int eventTypeId(CharcoalDbHelpers::getEventTypeId(
        m_connectionName, Enums::SupplyChainAction::LoadingAndTransport));

    if (eventTypeId == -1) {
        qWarning() << RED("Event Type ID not found!");
        return;
    }

    const ContinueEvent existingEvent(CharcoalDbHelpers::getContinueEvent(
        m_connectionName, eventTypeId));

    const QVariantMap props {
        { Tags::webPlateNumber, plateNumber },
        { Tags::webDestination, destinationId },
        { Tags::webQrCodes, scannedQrs },
        { Tags::webEventDate, eventDate.toSecsSinceEpoch() }
    };

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));

    if (existingEvent.eventId > 0) {
        // Event exists - we are continuing a paused event
        if (false == updateEvent(&query, existingEvent.eventId,
                                 existingEvent.entityId,
                                 eventTypeId, userId, timestamp,
                                 eventDate, coordinate,
                                 props,
                                 pauseEvent))
        {
            return;
        }
    } else {
        if (insertEntity(&query, typeId, transportId, parentEntityId) == false) {
            return;
        }

        // Then, insert a new Event under that Entity
        const int entityId(query.lastInsertId().toInt());

        if (false == insertEvent(&query, entityId, eventTypeId, userId, timestamp,
                                 eventDate, coordinate,
                                 props,
                                 pauseEvent))
        {
            return;
        }
    }

    emit refreshLocalEvents();
}

bool ActionController::registerReception(
    const QGeoCoordinate &coordinate,
    const QDateTime &timestamp,
    const QDateTime &eventDate,
    const QString &userId,
    const int transportId,
    const QVariantList &documents,
    const QVariantList &receipts,
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

    if (transportId == -1) {
        qWarning() << RED("Entity ID not found!");
        emit error(tr("Transport ID is incorrect %1").arg(transportId));
        return false;
    }

    const int eventTypeId(CharcoalDbHelpers::getEventTypeId(
        m_connectionName, Enums::SupplyChainAction::Reception));

    if (eventTypeId == -1) {
        qWarning() << RED("Event Type ID not found!");
        emit error(tr("Transport type is incorrect %1").arg(eventTypeId));
        return false;
    }

    // Special case: if this reception ALREADY EXISTS, we skip inserting it
    // into DB.
    // This can only happen for debug server where we have some dummy, unfinished
    // events
    const int existing = CharcoalDbHelpers::getInteger(
        m_connectionName, "Events",
        { Tags::entityId, Tags::typeId },
        { transportId, eventTypeId },
        Tags::id,
        QString(), false
        );

    if (existing != -1) {
        qWarning() << RED("Reception already exists - special case!")
                   << transportId << eventTypeId << existing;
        return true;
    }

    const QStringList cachedDocs(m_picturesManager->moveToCache(documents));
    const QStringList cachedRecs(m_picturesManager->moveToCache(receipts));

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));

    if (false == insertEvent(&query, transportId, eventTypeId, userId, timestamp,
                             eventDate, coordinate,
                             {
                                 { Tags::webDocuments, cachedDocs },
                                 { Tags::webReceipts, cachedRecs },
                                 { Tags::webQrCodes, scannedQrs },
                                 { Tags::webEventDate, eventDate.toSecsSinceEpoch() }
                             }))
    {
        emit error(tr("Failed to insert reception %1").arg(transportId));
        return false;
    }

    emit refreshLocalEvents();
    return true;
}

void ActionController::finalizeSupplyChain(const int transportId) const
{
    const int parentId(CharcoalDbHelpers::getParentEntityId(
        m_connectionName, transportId));
    const int grandParentId(CharcoalDbHelpers::getParentEntityId(
        m_connectionName, parentId));

    // Query below looks for all entities related to current transport. It will
    // also finalize harvests stemming from the same plot. However, it will
    // leave out transports coming from those parallel harvests. This is a bug!
    // Next time app synchronizes data from web, the DB will be fully corrected.

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));
    query.prepare("UPDATE Entities SET isFinished=1 WHERE "
                  "id=:transportId OR id=:parentId OR id=:grandParentId "
                  "OR parent=:parentId OR parent=:grandParentId");
    query.bindValue(":transportId", transportId);
    query.bindValue(":parentId", parentId);
    query.bindValue(":grandParentId", grandParentId);

    if (query.exec() == false) {
        qWarning() << RED("Finishing a supply chain has failed!")
                   << query.lastError().text()
                   << "for query:" << query.lastQuery()
                   << "with params:" << transportId << parentId;
        return;
    }

    // In online mode - send finalization call.
    // In offline mode - do nothing. Finalization will be handled by TrackingModel
    const auto webIds = CharcoalDbHelpers::getWebPackageIds(
        m_connectionName, transportId, parentId);

    qDebug() << "ENTITY HAS BEEN FINISHED!" << transportId << parentId << webIds;

    if (webIds.isEmpty() == false) {
        emit finalizePackages(webIds);
    }
}

void ActionController::registerReplantation(
    const QGeoCoordinate &coordinate,
    const QDateTime &timestamp,
    const QString &userId,
    const int plotId,
    const int numberOfTrees,
    const int treeSpeciesId,
    const QDateTime &beginningDate,
    const QDateTime &endingDate) const
{
    /*
     * Algorithm is:
     * - insert replantation entry into table
     * - mark parent entity (plot) as replanted (set isReplanted to 1)
     * - send action to web server
     */

    qDebug() << "Registering replantation" << coordinate << timestamp
             << userId << plotId << numberOfTrees
             << treeSpeciesId
             << beginningDate << endingDate;

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));

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
    query.bindValue(":plotId", plotId);
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
                   << "DB:" << m_connectionName;
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

int ActionController::scannedBagsForAction(const int transportId,
                                           const Enums::SupplyChainAction action) const
{
    const int eventTypeId(CharcoalDbHelpers::getEventTypeId(m_connectionName, action));

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));

    query.prepare("SELECT properties FROM Events WHERE typeId=:eventTypeId "
                  "AND entityId=:transportId");
    query.bindValue(":eventTypeId", eventTypeId);
    query.bindValue(":transportId", transportId);

    if (query.exec() == false) {
        qWarning() << RED("Getting total number of bags has failed!")
                   << query.lastError().text()
                   << "for query:" << query.lastQuery()
                   << "DB:" << m_connectionName;
        return -1;
    }

    int total = 0;
    while(query.next()) {
        const QByteArray propertiesString(query.value(Tags::properties).toByteArray());
        const QJsonObject properties(
            CharcoalDbHelpers::dbPropertiesToJson(propertiesString));
        total += properties.value(Tags::webQrCodes).toArray().size();
    }

    return total;
}

bool ActionController::insertEntity(QSqlQuery *query,
                                    const int typeId,
                                    const QString &packageId,
                                    const int parentId) const
{
    query->prepare("INSERT INTO Entities (typeId, name, parent, "
                   "isFinished, isReplanted) "
                   "VALUES (:typeId, :packageId, :parent, 0, 0)");
    query->bindValue(":typeId", typeId);
    query->bindValue(":packageId", packageId);
    query->bindValue(":parent", parentId == -1? QVariant(QVariant::Int) : parentId);

    if (query->exec() == false) {
        qWarning() << RED("Inserting new entity has failed!")
                   << query->lastError().text()
                   << "for query:" << query->lastQuery()
                   << "DB:" << m_connectionName
                   << typeId << packageId << parentId;
        return false;
    }

    return true;
}

bool ActionController::insertEvent(
    QSqlQuery *query,
    const int entityId,
    const int eventTypeId,
    const QString &userId,
    const QDateTime &timestamp,
    const QDateTime &eventDate,
    const QGeoCoordinate &coordinate,
    const QVariantMap &properties,
    const bool pauseEvent) const
{
    query->prepare("INSERT INTO Events (entityId, typeId, userId,"
                   "date, eventDate, locationLatitude, locationLongitude, properties, "
                   "isCommitted, isPaused) "
                   "VALUES (:entityId, :typeId, :userId, :date, :eventDate, "
                   ":locationLatitude, :locationLongitude, :properties, "
                   "0, :isPaused)");
    query->bindValue(":entityId", entityId);
    query->bindValue(":typeId", eventTypeId);
    query->bindValue(":userId", userId);
    query->bindValue(":date", timestamp.toSecsSinceEpoch());
    query->bindValue(":eventDate", eventDate.toSecsSinceEpoch());
    query->bindValue(":locationLatitude", coordinate.latitude());
    query->bindValue(":locationLongitude", coordinate.longitude());
    query->bindValue(":properties", CharcoalDbHelpers::propertiesToString(properties));
    query->bindValue(":isPaused", pauseEvent);

    if (query->exec() == false) {
        qWarning() << RED("Inserting event has failed!")
                   << query->lastError().text()
                   << "for query:" << query->lastQuery()
                   << entityId << eventTypeId << userId << timestamp << eventDate
                   << coordinate << properties;
        return false;
    }

    return true;
}

bool ActionController::updateEvent(
    QSqlQuery *query,
    const int eventId,
    const int entityId,
    const int eventTypeId,
    const QString &userId,
    const QDateTime &timestamp,
    const QDateTime &eventDate,
    const QGeoCoordinate &coordinate,
    const QVariantMap &properties,
    const bool pauseEvent) const
{
    query->prepare("UPDATE Events "
                   "SET entityId=:entityId, typeId=:typeId, userId=:userId,"
                   "date=:date, eventDate=:eventDate, "
                   "locationLatitude=:locationLatitude, locationLongitude=:locationLongitude, "
                   "properties=:properties, "
                   "isCommitted=0, isPaused=:isPaused "
                   "WHERE id=:eventId");
    query->bindValue(":entityId", entityId);
    query->bindValue(":typeId", eventTypeId);
    query->bindValue(":userId", userId);
    query->bindValue(":date", timestamp.toSecsSinceEpoch());
    query->bindValue(":eventDate", eventDate.toSecsSinceEpoch());
    query->bindValue(":locationLatitude", coordinate.latitude());
    query->bindValue(":locationLongitude", coordinate.longitude());
    query->bindValue(":properties", CharcoalDbHelpers::propertiesToString(properties));
    query->bindValue(":isPaused", pauseEvent);
    query->bindValue(":eventId", eventId);

    if (query->exec() == false) {
        qWarning() << RED("Updating paused event has failed!")
                   << query->lastError().text()
                   << "for query:" << query->lastQuery()
                   << entityId << eventTypeId << userId << timestamp << eventDate
                   << coordinate << properties;
        return false;
    }

    return true;
}
