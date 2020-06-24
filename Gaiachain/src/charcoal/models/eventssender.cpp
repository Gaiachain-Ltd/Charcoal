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

        const QString id(query().value("id").toString());
        const QString action(getEventType(query().value("typeId").toString()));

        const QJsonDocument doc(
            {
                { "pid", id },
                { "action", action },
                { "timestamp",
                 query().value("date").toDateTime().toSecsSinceEpoch() },
                { "location", location },
                { "properties", query().value("properties").toJsonObject() }
            });

        request->setDocument(doc);

        if (isLoggedIn) {
            qDebug() << "Sending replantation event!" << doc;
            request->setToken(m_sessionManager->token());
            m_sessionManager->sendRequest(request);
        } else {
            qDebug() << "Enqueuing replantation event!";
            m_queuedRequests.append(request);
        }
    }
}

void EventsSender::webErrorHandler(const QString &error,
                                   const QNetworkReply::NetworkError code)
{
    qDebug() << "Request error!" << error << code;
    // TODO: retry!
}

void EventsSender::webReplyHandler(const QJsonDocument &reply)
{
    qDebug() << "Request success!" << reply;

    const QString id(reply.object().value(Tags::pid).toString());
    const QString queryString(QString("UPDATE Events SET isCommitted=1 "
                                      "WHERE id=%1").arg(id));

    QSqlQuery query(queryString, db::Helpers::databaseConnection(m_connectionName));
    if (query.exec()) {
        // Not necessary?
        //emit webDataRefreshed();
    } else {
        qWarning() << RED("Query to update the Replantations has failed to execute")
                   << query.lastError() << "For query:" << query.lastQuery();
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
