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

        const QString entityId(query().value("entityId").toString());
        const QString pid(getEntityName(entityId));
        const QString action(getEventType(query().value("typeId").toString()));

        const QJsonDocument doc(
            {
                { "pid", pid },
                { "action", action },
                { "timestamp",
                 static_cast<qint64>(query().value("date").toLongLong()) },
                { "location", location },
                { "properties", dbMapToWebObject(query().value("properties").toString()) }
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

    const QString id(reply.object().value(Tags::pid).toString());
    const QString eventId(reply.object().value(Tags::eventId).toString());
    // Will become incorrect after year 2038!
    const qint64 timestamp(reply.object().value(Tags::eventTimestamp).toInt());

    const QString queryString(QString("UPDATE Events SET isCommitted=1 "
                                      "WHERE date=%1").arg(timestamp));

    QSqlQuery query(queryString, db::Helpers::databaseConnection(m_connectionName));
    //query.prepare(queryString);
    //query.bindValue(":date", timestamp);

    if (query.exec()) {
        query.next();
        qDebug() << "Updated?" << query.lastQuery() << query.value(0) << timestamp;
        // Not necessary?
        //emit webDataRefreshed();
    } else {
        qWarning() << RED("Query to update the Event has failed to execute")
                   << query.lastError() << "For query:" << query.lastQuery()
                   << timestamp;
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
