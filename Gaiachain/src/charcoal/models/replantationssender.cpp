#include "replantationssender.h"

#include "common/tags.h"
#include "common/logs.h"
#include "rest/baserequest.h"
#include "controllers/session/restsessionmanager.h"
#include "controllers/usermanager.h"
#include "helpers/requestshelper.h"
#include "database/dbhelpers.h"
#include "charcoal/database/charcoaldbhelpers.h"

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

    const bool isLoggedIn = RequestsHelper::isOnline(m_sessionManager.get(),
                                                     m_userManager.get());
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
        const qint64 beginning = query().value("beginningDate").toLongLong();
        const qint64 ending = query().value("endingDate").toLongLong();
        const int treeSpecies = query().value("treeSpecies").toInt();
        const int parentId(query().value("plotId").toInt());
        const int webPlotId(CharcoalDbHelpers::getWebPackageId(m_connectionName, parentId));

        const QJsonDocument doc(
            {
                { "trees_planted", numberOfTrees },
                { Tags::webBeginningDate, beginning },
                { Tags::webEndingDate, ending },
                { Tags::location, location },
                { "plot", webPlotId },
                { Tags::webTreeSpecies, treeSpecies }
            });

        request->setDocument(doc);

        if (isLoggedIn && i == 0) {
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
    emit error(errorString);

    continueSendingQueuedRequests();
}

void ReplantationsSender::webReplyHandler(const QJsonDocument &reply)
{
    qDebug() << "Request success!" << reply;

    const qint64 begin(reply.object().value(Tags::webBeginningDate).toVariant().toLongLong());
    const qint64 end(reply.object().value(Tags::webEndingDate).toVariant().toLongLong());
    const QString queryString(
        QString("UPDATE Replantations SET isCommitted=1 "
                "WHERE beginningDate=%1 AND endingDate=%2").arg(begin).arg(end));

    QSqlQuery query(queryString, db::Helpers::databaseConnection(m_connectionName));
    if (query.exec() == false) {
        const QLatin1String errorString = QLatin1String("Query to update the Replantation has failed to execute");
        qWarning() << RED(errorString)
                   << query.lastError() << "For query:" << query.lastQuery()
                   << reply;
        emit error(errorString);
    }

    continueSendingQueuedRequests();
}
