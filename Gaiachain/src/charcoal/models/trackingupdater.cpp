#include "trackingupdater.h"

#include "charcoal/database/charcoaldbhelpers.h"
#include "database/dbhelpers.h"
#include "common/logs.h"

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
    const QString pid(object.value("pid").toString());
    const QString webType(object.value("type").toString());
    const auto type = packageType(webType);
    const int typeId(CharcoalDbHelpers::getEntityTypeId(m_connectionName, type));
    // Not parsed and not needed: "type_display" property
    const QJsonArray events(object.value("entities").toArray());

    const int entityId = CharcoalDbHelpers::getEntityIdFromWebId(
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
        query.bindValue(":parent", parentEntityId);

        if (query.exec() == false) {
            qWarning() << RED("Inserting Entity has failed!")
                       << query.lastError().text()
                       << "for query:" << query.lastQuery();
            return false;
        }
    }

    /*
    for (int i = events.size() - 1; i >= 0; --i) {
        const QJsonObject event(events.at(i).toObject());
        // Warning: will become invalid in the year 2038!
        const qint64 timestamp = event.value("timestamp").toInt();
        const QJsonArray location = event.value("location_display").toArray();

        // TODO: check if event exists!

        QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));
        query.prepare("INSERT INTO Events (entityId, typeId, userId, "
                      "date, eventDate, locationLatitude, locationLongitude, "
                      "properties, isCommitted) "
                      "VALUES (:entityId, :typeId, :userId, :date, :eventDate, "
                      ":locationLatitude, :locationLongitude, :properties, 0)");
        query.bindValue(":entityId", entityId);
        // WEB: lacking info!
        //query.bindValue(":typeId", eventTypeId);
        // WEB: lacking info!
        //query.bindValue(":userId", userId);
        // WEB: lacking info!
        query.bindValue(":date", timestamp);
        query.bindValue(":eventDate", timestamp);
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
    */

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
