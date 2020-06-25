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
    setDbQuery("SELECT id, plotId, userId, numberOfTrees, treeSpecies, "
               "locationLatitude, locationLongitude, beginningDate, endingDate "
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

        // We have to read all data first, and only then we can call
        // getWebPackageId() because it invalidates current query()
        const auto numberOfTrees(query().value("numberOfTrees").toString());
        const qint64 beginning = static_cast<qint64>(
            query().value("beginningDate").toLongLong());
        const qint64 ending = static_cast<qint64>(
            query().value("endingDate").toLongLong());
        const int treeSpecies = query().value("treeSpecies").toInt();

        const QJsonDocument doc(
            {
                { "trees_planted", numberOfTrees },
                { "beginning_date", beginning },
                { "ending_date", ending },
                { "location", location },
                { "plot", getWebPackageId(query().value("plotId").toString()) },
                { "tree_specie", treeSpecies }
            });

        request->setDocument(doc);

        if (isLoggedIn) {
            qDebug() << "Sending replantation event!" << doc;
            request->setToken(m_sessionManager->token());
            m_sessionManager->sendRequest(request, this,
                                          &ReplantationsSender::webErrorHandler,
                                          &ReplantationsSender::webReplyHandler);
        } else {
            qDebug() << "Enqueuing replantation event!" << doc;
            m_queuedRequests.append(request);
        }
    }
}

void ReplantationsSender::webErrorHandler(const QString &errorString,
                                          const QNetworkReply::NetworkError code)
{
    qDebug() << "Request error!" << errorString << code;
    // TODO: retry!
}

void ReplantationsSender::webReplyHandler(const QJsonDocument &reply)
{
    qDebug() << "Request success!" << reply;

    const qint64 begin(reply.object().value(Tags::beginningDate).toInt());
    const qint64 end(reply.object().value(Tags::endingDate).toInt());
    const QString queryString(
        QString("UPDATE Replantations SET isCommitted=1 "
                "WHERE beginningDate=%1 AND endingDate=%2").arg(begin).arg(end));

    QSqlQuery query(queryString, db::Helpers::databaseConnection(m_connectionName));
    if (query.exec() == false) {
        qWarning() << RED("Query to update the Replantation has failed to execute")
                   << query.lastError() << "For query:" << query.lastQuery()
                   << reply;
    }
}

int ReplantationsSender::getWebPackageId(const QString &plotEntityId) const
{
    int result = -1;
    const QString queryString(QString("SELECT webId FROM Entities "
                                      "WHERE id=%1").arg(plotEntityId));

    QSqlQuery query(queryString, db::Helpers::databaseConnection(m_connectionName));
    if (query.exec() && query.next()) {
        result = query.value("webId").toInt();
    } else {
        qWarning() << RED("Unable to fetch package ID (Web)")
                   << query.lastError() << "For query:" << query.lastQuery();
    }

    return result;
}
