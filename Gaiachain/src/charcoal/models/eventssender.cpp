#include "eventssender.h"

#include "common/tags.h"
#include "common/logs.h"
#include "rest/baserequest.h"
#include "controllers/session/restsessionmanager.h"
#include "controllers/usermanager.h"
#include "database/dbhelpers.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

#include <QJsonObject>

EventsSender::EventsSender(QObject *parent) : QueryModel(parent)
{
    setWebModelCanChange(false);
    setDbQuery("SELECT id, entityId, typeId, userId, date, "
               "locationLatitude, locationLongitude, properties "
               "FROM Events WHERE isCommitted=0");

    connect(this, &EventsSender::refreshed,
            this, &EventsSender::sendEvents);
}

void EventsSender::sendEvents()
{
    const int count = rowCount();
    if (count == 0) {
        return;
    }

    const bool isLoggedIn = m_userManager->isLoggedIn();
    for (int i = 0; i < count; ++i) {
        // use query().next()?
        if (query().seek(i) == false) {
            continue;
        }

        const auto record = query().record();
        qDebug() << "Offline event is:" << record;

        const auto request = QSharedPointer<BaseRequest>::create(
            "/entities/new/",
            BaseRequest::Type::Post
            );

        const QJsonObject location({
            { "latitude", query().value("locationLatitude").toDouble() },
            { "longitude", query().value("locationLongitude").toDouble() }
        });

        // First, get query data. Subsequent calls to any QSqlQuery instances
        // might break the main query!
        const qint64 timestamp =
            static_cast<qint64>(query().value("date").toLongLong());
        const QString properties(query().value("properties").toString());

        const QString entityId(query().value("entityId").toString());
        const QString typeId(query().value("typeId").toString());

        // Now generate stuff using additional QSqlQueries
        const QString pid(getEntityName(entityId));
        const QString action(getEventType(typeId));

        const QJsonDocument doc(
            {
                { "pid", pid },
                { "action", action },
                { "timestamp", timestamp },
                { "location", location },
                { "properties", dbMapToWebObject(properties) }
            });

        request->setDocument(doc);

        if (isLoggedIn) {
            qDebug().noquote() << "Sending event!" << doc;
            request->setToken(m_sessionManager->token());
            m_sessionManager->sendRequest(request, this,
                                          &EventsSender::webErrorHandler,
                                          &EventsSender::webReplyHandler);
        } else {
            qDebug().noquote() << "Enqueuing event!" << doc;
            m_queuedRequests.append(request);
        }
    }
}

void EventsSender::webErrorHandler(const QString &errorString,
                                   const QNetworkReply::NetworkError code)
{
    qDebug() << "Request error!" << errorString << code;
    emit error(errorString);
}

void EventsSender::webReplyHandler(const QJsonDocument &reply)
{
    qDebug() << "Request success!" << reply;

    const QString pid(reply.object().value(Tags::pid).toString());
    const qint64 eventWebId(reply.object().value(Tags::eventId).toInt());
    // Will become incorrect after year 2038!
    const qint64 timestamp(reply.object().value(Tags::eventTimestamp).toInt());
    const QString eventId(findEventByTimestamp(timestamp));

    const QString queryString(QString("UPDATE Events SET isCommitted=1 "
                                      "WHERE id=%1").arg(eventId));

    QSqlQuery query(queryString, db::Helpers::databaseConnection(m_connectionName));

    if (query.exec()) {
        if (updateEntityWebId(eventWebId, eventId) == false) {
            const QLatin1String errorString = QLatin1String("Failed to update local DB");
            qWarning() << RED(errorString)
                       << query.lastError() << "For query:" << query.lastQuery()
                       << reply;
            emit error(errorString);
        }
    } else {
        const QLatin1String errorString = QLatin1String("Query to update the Event has failed to execute");
        qWarning() << RED(errorString)
                   << query.lastError() << "For query:" << query.lastQuery()
                   << timestamp << "pid" << pid << "eid" << eventWebId;
        emit error(errorString);
    }
}

QString EventsSender::getEventType(const QString &id) const
{
    QString result;
    const QString queryString(QString("SELECT actionName FROM EventTypes "
                                      "WHERE id=%1").arg(id));
    QSqlQuery query(queryString, db::Helpers::databaseConnection(m_connectionName));
    if (query.exec() && query.next()) {
        result = query.value("actionName").toString();
    } else {
        qWarning() << RED("Unable to fetch event type (action)")
                   << query.lastError() << "For query:" << query.lastQuery();
    }

    return result;
}

QString EventsSender::findEventByTimestamp(const qint64 timestamp) const
{
    QString result;
    const QString queryString(QString("SELECT id FROM Events "
                                      "WHERE date=%1").arg(timestamp));

    QSqlQuery query(queryString, db::Helpers::databaseConnection(m_connectionName));
    if (query.exec() && query.next()) {
        result = query.value("id").toString();
    } else {
        qWarning() << RED("Unable to find event ID by timestamp")
                   << query.lastError() << "For query:" << query.lastQuery();
    }

    return result;
}

bool EventsSender::updateEntityWebId(const qint64 webId, const QString &eventId) const
{
    QString result;
    const QString queryString(QString("UPDATE Entities SET webId=%1 "
                                      "WHERE id IN (SELECT entityId FROM Events WHERE id=%2)")
                                  .arg(webId).arg(eventId));

    QSqlQuery query(queryString, db::Helpers::databaseConnection(m_connectionName));
    if (query.exec()) {
        return true;
    } else {
        qWarning() << RED("Unable to update web ID")
                   << query.lastError() << "For query:" << query.lastQuery();
    }

    return false;
}

QString EventsSender::getEntityName(const QString &id) const
{
    QString result;
    const QString queryString(QString("SELECT name FROM Entities "
                                      "WHERE id=%1").arg(id));

    QSqlQuery query(queryString, db::Helpers::databaseConnection(m_connectionName));
    if (query.exec() && query.next()) {
        result = query.value("name").toString();
    } else {
        qWarning() << RED("Unable to fetch entity name (plot ID, harvest ID or transport ID")
                   << query.lastError() << "For query:" << query.lastQuery();
    }

    return result;
}

/*!
 * This method takes \a properties from Events database and transformes
 * them into a JSON object understood by Web.
 */
QJsonObject EventsSender::dbMapToWebObject(const QString &properties) const
{
    const QJsonDocument propertiesDoc(QJsonDocument::fromJson(properties.toLatin1()));
    return propertiesDoc.object();
}
