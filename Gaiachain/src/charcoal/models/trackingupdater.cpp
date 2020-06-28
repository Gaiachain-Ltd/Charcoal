#include "trackingupdater.h"

#include "charcoal/database/charcoaldbhelpers.h"
#include "database/dbhelpers.h"
#include "common/logs.h"
#include "common/tags.h"

#include <QVariant>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <QSqlQuery>
#include <QSqlError>

TrackingUpdater::TrackingUpdater(const QString &connectionName)
    : m_connectionName(connectionName)
{
    if (isValid() == false) {
        qWarning() << RED("Invalid table name or connection name!")
                   << m_connectionName;
    }
}

bool TrackingUpdater::updateTable(const QJsonDocument &json) const
{
    const QJsonObject mainObject(json.object());
    const QJsonArray mainArray(mainObject.value("results").toArray());

    // We go back-to-front, in order to register oldest events first
    for (int i = mainArray.size() - 1; i >= 0; --i) {
        if (processTrackingItem(mainArray.at(i).toObject()) == false) {
            return false;
        }
    }

    return true;
}

bool TrackingUpdater::isValid() const
{
    return m_connectionName.isEmpty() == false;
}

bool TrackingUpdater::processTrackingItem(const QJsonObject &object) const
{
    const int webId(object.value("id").toInt(-1));
    const QString pid(object.value(Tags::pid).toString());
    const QString webType(object.value("type").toString());
    const auto type = packageType(webType);
    const int typeId(CharcoalDbHelpers::getEntityTypeId(m_connectionName, type));
    // Not parsed and not needed: "type_display" property
    const QJsonArray events(object.value("entities").toArray());

    int entityId = CharcoalDbHelpers::getEntityIdFromWebId(
        m_connectionName, webId, false);
    const int parentEntityId = CharcoalDbHelpers::getEntityIdFromName(
        m_connectionName, pid, false);

    if (entityId == -1) {
        // Entity does not exist in our DB - add it!
        QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));
        query.prepare("INSERT INTO Entities (typeId, name, parent, "
                      "isFinished, isReplanted) "
                      "VALUES (:typeId, :name, :parent, 0, 0)");
        query.bindValue(":typeId", typeId);
        query.bindValue(":name", pid);
        if (parentEntityId != -1) {
            query.bindValue(":parent", parentEntityId);
        } else {
            query.bindValue(":parent", 0);
        }

        if (query.exec() == false) {
            qWarning() << RED("Inserting Entity has failed!")
                       << query.lastError().text()
                       << "for query:" << query.lastQuery();
            return false;
        }

        entityId = query.lastInsertId().toInt();
    }

    for (int i = events.size() - 1; i >= 0; --i) {
        const QJsonObject event(events.at(i).toObject());
        const int eventWebId(object.value("id").toInt(-1));
        const qint64 timestamp = event.value(Tags::timestamp).toString().toLongLong();
        const qint64 eventDate = event.value(Tags::webEventDate).toString().toLongLong();
        const QJsonArray location = event.value("location_display").toArray();
        const int eventTypeId = CharcoalDbHelpers::getEventTypeId(
            m_connectionName, event.value("action").toString());
        const QString userId(event.value("user_code").toString());

        const int eventId = CharcoalDbHelpers::getEventIdFromWebId(
            m_connectionName, eventWebId, false);

        if (eventId == -1) {
            QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));
            query.prepare("INSERT INTO Events (entityId, typeId, userId, webId, "
                          "date, eventDate, locationLatitude, locationLongitude, "
                          "properties, isCommitted) "
                          "VALUES (:entityId, :typeId, :userId, :eventWebId, "
                          ":date, :eventDate, :locationLatitude, :locationLongitude, "
                          ":properties, 0)");
            query.bindValue(":entityId", entityId);
            query.bindValue(":typeId", eventTypeId);
            query.bindValue(":userId", userId);
            query.bindValue(":eventWebId", eventWebId);
            query.bindValue(":date", timestamp);
            query.bindValue(":eventDate", eventDate);
            query.bindValue(":locationLatitude", location.at(0));
            query.bindValue(":locationLongitude", location.at(1));
            // WEB: lacking info!
            //        query.bindValue(":properties",
            //                        propertiesToString(QVariantMap {
            //                            { Tags::webOvenType, ovenType },
            //                            { Tags::webEventDate, eventDate.toSecsSinceEpoch() },
            //                            { Tags::webPlotId, webPlotId },
            //                            { Tags::webOvenId, ovenId }
            //                        }));

            if (query.exec() == false) {
                qWarning() << RED("Inserting event has failed!")
                           << query.lastError().text() << "for query:" << query.lastQuery();
                return false;
            }
        }
    }

    // TODO: update Entities and Events if they already exist!
    // TODO: schedule loading of "properties" for Events

    return true;
}

Enums::PackageType TrackingUpdater::packageType(const QString &webType) const
{
    if (webType == QLatin1String("TRK")) {
        return Enums::PackageType::Transport;
    } else if (webType == QLatin1String("HAR")) {
        return Enums::PackageType::Harvest;
    } else if (webType == QLatin1String("PLT")) {
        return Enums::PackageType::Plot;
    }

    return Enums::PackageType::Unknown;
}
