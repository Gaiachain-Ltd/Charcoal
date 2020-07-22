#include "unusedharvestidsmodel.h"

#include "common/tags.h"
#include "common/logs.h"
#include "database/dbhelpers.h"
#include "charcoal/database/charcoaldbhelpers.h"

#include <QSqlQuery>
#include <QSqlError>

#include <QJsonArray>

UnusedHarvestIdsModel::UnusedHarvestIdsModel(QObject *parent) : SimpleListQueryModel(parent)
{
    setWebModelCanChange(true);
    setDbQuery("SELECT id, name FROM Entities WHERE isFinished=0 AND typeId IN "
               "(SELECT id FROM EntityTypes WHERE name=\"Harvest\")");
}

bool UnusedHarvestIdsModel::hasPausedEvent() const
{
    return m_hasPausedEvent;
}

void UnusedHarvestIdsModel::checkForPausedEvent()
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
}

QString UnusedHarvestIdsModel::harvestId() const
{
    return CharcoalDbHelpers::getHarvestName(m_pausedEntity.name);
}

QGeoCoordinate UnusedHarvestIdsModel::location() const
{
    return m_pausedEvent.location;
}

QString UnusedHarvestIdsModel::plateNumber() const
{
    return m_pausedEvent.properties.value(Tags::webPlateNumber).toString();
}

QString UnusedHarvestIdsModel::destination() const
{
    return CharcoalDbHelpers::getDestinationName(
        m_connectionName,
        m_pausedEvent.properties.value(Tags::webDestination).toInt());
}

QVariantList UnusedHarvestIdsModel::scannedQrs() const
{
    return m_pausedEvent.properties.value(Tags::webQrCodes).toArray().toVariantList();
}

QDateTime UnusedHarvestIdsModel::loadingDate() const
{
    return QDateTime::fromSecsSinceEpoch(
                m_pausedEvent.properties.value(Tags::webEventDate).toVariant().toLongLong());
}

QDateTime UnusedHarvestIdsModel::timestamp() const
{
    return QDateTime::fromSecsSinceEpoch(m_pausedEvent.timestamp);
}

void UnusedHarvestIdsModel::refreshWebData()
{
    checkForPausedEvent();
    QueryModel::refreshWebData();
}
