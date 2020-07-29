#include "unusedharvestidsmodelfortransport.h"

#include "common/tags.h"
#include "common/logs.h"
#include "database/dbhelpers.h"
#include "charcoal/database/charcoaldbhelpers.h"

#include <QSqlQuery>
#include <QSqlError>

#include <QJsonArray>

UnusedHarvestIdsModelForTransport::UnusedHarvestIdsModelForTransport(QObject *parent)
    : SimpleListQueryModel(false, parent)
{
    setWebModelCanChange(true);
    setDbQuery("SELECT id, name FROM Entities WHERE isFinished=0 "
               "AND typeId IN "
               "(SELECT id FROM EntityTypes WHERE name=\"Harvest\") "
               "AND (SELECT COUNT(id) = 0 FROM Ovens "
               "WHERE carbonizationEnding IS NULL AND plot=Entities.parent)");
    // We only show harvests which have all their ovens fully carbonized
}

bool UnusedHarvestIdsModelForTransport::hasPausedEvent() const
{
    return m_hasPausedEvent;
}

void UnusedHarvestIdsModelForTransport::checkForPausedEvent()
{
    QSqlQuery q(QString(), db::Helpers::databaseConnection(m_connectionName));
    q.prepare("SELECT id, entityId, typeId, userId, date, eventDate, "
              "locationLatitude, locationLongitude, properties "
              "FROM Events "
              "WHERE isPaused=1");

    if (q.exec()) {
        if (q.next()) {
            if (m_hasPausedEvent == false) {
                m_hasPausedEvent = true;
                m_pausedEvent.loadFromQuery(&q);
                q.finish();
                m_pausedEntity = Entity();
                m_pausedEntity.loadFromDb(m_connectionName, m_pausedEvent.entityId);
                emit hasPausedEventChanged(m_hasPausedEvent);
            }
        } else {
            if (m_hasPausedEvent == true) {
                m_pausedEvent = Event();
                m_pausedEntity = Entity();
                m_hasPausedEvent = false;
                emit hasPausedEventChanged(m_hasPausedEvent);
            }
        }
    } else {
        qWarning() << RED("Checking for paused events has failed")
                   << q.lastError().text()
                   << "for query:" << q.lastQuery()
                   << "DB:" << m_connectionName;
    }

    qDebug() << "Checking for paused events" << m_hasPausedEvent
             << m_pausedEvent.id << m_pausedEntity.name;
}

QString UnusedHarvestIdsModelForTransport::harvestName() const
{
    return CharcoalDbHelpers::getHarvestName(m_pausedEntity.name);
}

int UnusedHarvestIdsModelForTransport::harvestId() const
{
    return m_pausedEntity.id;
}

QGeoCoordinate UnusedHarvestIdsModelForTransport::location() const
{
    return m_pausedEvent.location;
}

QString UnusedHarvestIdsModelForTransport::plateNumber() const
{
    return m_pausedEvent.properties.value(Tags::webPlateNumber).toString();
}

QString UnusedHarvestIdsModelForTransport::destination() const
{
    return CharcoalDbHelpers::getDestinationName(
        m_connectionName,
                m_pausedEvent.properties.value(Tags::webDestination).toInt());
}

int UnusedHarvestIdsModelForTransport::destinationId() const
{
    return m_pausedEvent.properties.value(Tags::webDestination).toInt();
}

QVariantList UnusedHarvestIdsModelForTransport::scannedQrs() const
{
    return m_pausedEvent.properties.value(Tags::webQrCodes).toArray().toVariantList();
}

QDateTime UnusedHarvestIdsModelForTransport::loadingDate() const
{
    return QDateTime::fromSecsSinceEpoch(
                m_pausedEvent.properties.value(Tags::webEventDate).toVariant().toLongLong());
}

QDateTime UnusedHarvestIdsModelForTransport::timestamp() const
{
    return QDateTime::fromSecsSinceEpoch(m_pausedEvent.timestamp);
}

void UnusedHarvestIdsModelForTransport::refreshWebData()
{
    checkForPausedEvent();
    QueryModel::refreshWebData();
}
