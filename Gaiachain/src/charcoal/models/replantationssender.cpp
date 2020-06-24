#include "replantationssender.h"

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

ReplantationsSender::ReplantationsSender(QObject *parent) : QueryModel(parent)
{
    setWebModelCanChange(false);
    setDbQuery("SELECT id, plotId, userId, numberOfTrees, locationLatitude, "
               "locationLongitude, beginningDate "
               "FROM Replantations WHERE isCommitted=0");

    connect(this, &ReplantationsSender::refreshed,
            this, &ReplantationsSender::sendEvents);
}

void ReplantationsSender::sendEvents()
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
        qDebug() << "Offline replantation is:" << record;

        const auto request = QSharedPointer<BaseRequest>::create(
            "/entities/replantation/",
            BaseRequest::Type::Post
            );

        const QJsonObject location({
            { "latitude", query().value("locationLatitude").toDouble() },
            { "longitude", query().value("locationLongitude").toDouble() }
        });

        const QJsonDocument doc(
            {
                { "trees_planted", query().value("numberOfTrees").toString() },
                { "beginning_date",
                 query().value("beginningDate").toDateTime().toSecsSinceEpoch() },
                { "ending_date",
                 query().value("endingDate").toDateTime().toSecsSinceEpoch()},
                { "location", location },
                { "plot", query().value("plotId").toInt() },
                { "tree_specie", query().value("treeSpecies").toInt() }
            });

        request->setDocument(doc);

        if (isLoggedIn) {
            qDebug() << "Sending replantation event!" << doc;
            request->setToken(m_sessionManager->token());
            m_sessionManager->sendRequest(request, this,
                                          &ReplantationsSender::webErrorHandler,
                                          &ReplantationsSender::webReplyHandler);
        } else {
            qDebug() << "Enqueuing replantation event!";
            m_queuedRequests.append(request);
        }
    }
}

void ReplantationsSender::webErrorHandler(const QString &error,
                                          const QNetworkReply::NetworkError code)
{
    qDebug() << "Request error!" << error << code;
    // TODO: retry!
}

void ReplantationsSender::webReplyHandler(const QJsonDocument &reply)
{
    qDebug() << "Request success!" << reply;

    const QString id(reply.object().value(Tags::pid).toString());
    const QString queryString(QString("UPDATE Replantations SET isCommitted=1 "
                                      "WHERE id=%1").arg(id));

    QSqlQuery query(queryString, db::Helpers::databaseConnection(m_connectionName));
    if (query.exec()) {
        // Not necessary?
        //emit webDataRefreshed();
    } else {
        qWarning() << RED("Query to update the Replantation has failed to execute")
                   << query.lastError() << "For query:" << query.lastQuery();
    }
}
