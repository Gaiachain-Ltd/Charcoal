#include "eventssender.h"

#include "common/tags.h"
#include "common/logs.h"
#include "helpers/requestshelper.h"
#include "rest/baserequest.h"
#include "controllers/session/restsessionmanager.h"
#include "controllers/usermanager.h"
#include "database/dbhelpers.h"
#include "charcoal/database/charcoaldbhelpers.h"
#include "charcoal/rest/multipartrequest.h"
#include "charcoal/rest/imagerequest.h"
#include "charcoal/picturesmanager.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

#include <QJsonObject>
#include <QJsonArray>

EventsSender::EventsSender(QObject *parent) : QueryModel(parent)
{
    setWebModelCanChange(false);
    setDbQuery("SELECT id, entityId, typeId, userId, date, "
               "locationLatitude, locationLongitude, properties "
               "FROM Events WHERE isCommitted=0 AND isPaused=0");

    connect(this, &EventsSender::refreshed,
            this, &EventsSender::sendEvents);
}

void EventsSender::setPicturesManager(PicturesManager *manager)
{
    m_picturesManager = manager;
    if (m_picturesManager) {
        connect(manager, &PicturesManager::fetchPhoto,
                this, &EventsSender::onFetchPhoto);
    }
}

bool EventsSender::hasQueuedRequests() const
{
    return rowCount() > 0 || QueryModel::hasQueuedRequests();
}

void EventsSender::sendQueuedRequests()
{
    if (QueryModel::hasQueuedRequests()) {
        QueryModel::sendQueuedRequests();
    }

    if (rowCount() > 0) {
        sendEvents();
    }
}

void EventsSender::sendEvents()
{
    if (canSendNextEvent()) {
        sendEvent();
    }
}

void EventsSender::onFinalizePackage(const int webId)
{
    const auto request = QSharedPointer<BaseRequest>::create(
        QString("/entities/packages/%1/finalize_supply_chain/").arg(webId),
        BaseRequest::Type::Post);

    if (RequestsHelper::isOnline(m_sessionManager.get(), m_userManager.get())) {
        qDebug() << "Sending finalization event!";
        request->setToken(m_sessionManager->token());
        m_sessionManager->sendRequest(request, this,
                                      &EventsSender::webErrorHandler,
                                      &EventsSender::finalizationReplyHandler);
    }

    // Finalization events are not queued - they are either sent directly
    // or will be run after last reception from a package is processed
}

void EventsSender::onFinalizePackages(const QVector<int> &webIds)
{
    for (const int webId : webIds) {
        onFinalizePackage(webId);
    }
}

void EventsSender::webErrorHandler(const QString &errorString,
                                   const QNetworkReply::NetworkError code)
{
    qDebug() << "Request error!" << errorString << code;
    emit error(errorString);

    m_hasPendingEvent = false;
    continueSendingQueuedRequests();
}

void EventsSender::webReplyHandler(const QJsonDocument &reply)
{
    qDebug() << "Request success!" << reply;

    const QString pid(reply.object().value(Tags::pid).toString());
    const qint64 entityWebId(reply.object().value(Tags::webEventId).toInt());
    const qint64 timestamp(reply.object().value(Tags::eventTimestamp)
                               .toVariant().toLongLong());
    const int eventId(CharcoalDbHelpers::getEventId(m_connectionName, timestamp));

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));

    query.prepare("UPDATE Events "
                  "SET isCommitted=1, parentWebId=:entityWebId "
                  "WHERE date=:timestamp");
    query.bindValue(":entityWebId", entityWebId);
    query.bindValue(":timestamp", timestamp);

    if (query.exec()) {
        query.finish();
        if (updateEntityWebId(entityWebId, eventId) == false) {
            const QLatin1String errorString = QLatin1String("Failed to update local DB");
            qWarning() << RED(errorString)
                       << query.lastError() << "For query:" << query.lastQuery()
                       << reply;
            emit error(errorString);
        }

        // Only run this for RECEPTION events!
        const Enums::SupplyChainAction action = CharcoalDbHelpers::getEventTypeFromEventId(
            m_connectionName, eventId);

        if (action == Enums::SupplyChainAction::Reception) {
            // Check if package needs to be finalized
            query.prepare("SELECT isFinished FROM Entities "
                          "WHERE id IN (SELECT entityId FROM Events "
                          "WHERE id=:eventId)");
            query.bindValue(":eventId", eventId);

            if (query.exec() && query.next()) {
                if (query.value("isFinished").toInt() == 1) {
                    onFinalizePackage(entityWebId);
                }
            } else {
                const QLatin1String errorString = QLatin1String(
                    "Failed to check if package is finalized");
                qWarning() << RED(errorString)
                           << query.lastError() << "For query:" << query.lastQuery()
                           << reply;
                emit error(errorString);
            }

            query.finish();
        }
    } else {
        const QLatin1String errorString = QLatin1String("Query to update the Event has failed to execute");
        qWarning() << RED(errorString)
                   << query.lastError() << "For query:" << query.lastQuery()
                   << timestamp << Tags::pid << pid << "eid" << entityWebId;
        emit error(errorString);
    }

    m_hasPendingEvent = false;
    setQuery(m_query, db::Helpers::databaseConnection(m_connectionName));

    continueSendingQueuedRequests();
}

void EventsSender::finalizationReplyHandler(const QJsonDocument &reply)
{
    qDebug() << "Finalization successful" << reply;
}

void EventsSender::onFetchPhoto(const QString &path)
{
    QString adjusted;
    const QString prefix("/media/photos/");

    if (path.contains(prefix)) {
        adjusted = path;
    } else {
        adjusted.append(prefix);

        QString justDateTime(path);
        justDateTime = justDateTime.mid(path.indexOf('-', 0) + 1);
        justDateTime.truncate(justDateTime.lastIndexOf('.'));

        const QDateTime timestamp(QDateTime::fromString(
            justDateTime, "yyyy-MM-ddTHHmmss"));
        adjusted.append(timestamp.toString("yyyy/MM/dd"));
        adjusted.append("/");
        adjusted.append(path);
    }

    qDebug() << "Fetch photo:" << adjusted;
    const auto request = QSharedPointer<ImageRequest>::create(
        adjusted, m_picturesManager->cachePath());

    if (RequestsHelper::isOnline(m_sessionManager.get(), m_userManager.get())) {
        m_sessionManager->sendRequest(request);
    } else {
        qDebug() << "Enqueing request";
        m_queuedRequests.append(request);
    }
}

bool EventsSender::updateEntityWebId(const qint64 webId, const int eventId) const
{
    QString result;
    const QString queryString(QString("UPDATE Entities SET webId=%1 "
                                      "WHERE id IN (SELECT entityId FROM Events "
                                      "WHERE id=%2)")
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

/*!
 * This method takes \a properties from Events database and transformes
 * them into a JSON object understood by Web.
 */
QJsonObject EventsSender::dbMapToWebObject(QJsonObject object) const
{
    if (object.value(Tags::webDocuments).isNull() == false) {
        // We don't need to pass documents in properties, we send them separately
        object.remove(Tags::webDocuments);
    }

    if (object.value(Tags::webReceipts).isNull() == false) {
        // We don't need to pass receipts in properties, we send them separately
        object.remove(Tags::webReceipts);
    }

    return object;
}

bool EventsSender::canSendNextEvent() const
{
    return (m_hasPendingEvent == false)
        && RequestsHelper::isOnline(m_sessionManager.get(), m_userManager.get());
}

void EventsSender::sendEvent()
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

        // First, get query data. Subsequent calls to any QSqlQuery instances
        // might break the main query!
        const qint64 timestamp =
            static_cast<qint64>(query().value(Tags::date).toLongLong());
        const QString properties(query().value(Tags::properties).toString());

        const int entityId(query().value(Tags::entityId).toInt());
        const int typeId(query().value(Tags::typeId).toInt());

        const QJsonObject location({
            { Tags::latitude, query().value(Tags::locationLatitude).toDouble() },
            { Tags::longitude, query().value(Tags::locationLongitude).toDouble() }
        });

        const QJsonObject propertiesObject(
            CharcoalDbHelpers::dbPropertiesToJson(properties));
        QStringList toUpload;

        if (propertiesObject.contains(Tags::webDocuments)) {
            const auto array = propertiesObject.value(Tags::webDocuments).toArray();
            for (const auto &item : array) {
                toUpload.append(item.toString());
            }
        }

        if (propertiesObject.contains(Tags::webReceipts)) {
            const auto array = propertiesObject.value(Tags::webReceipts).toArray();
            for (const auto &item : array) {
                toUpload.append(item.toString());
            }
        }

        if (toUpload.isEmpty() == false) {
            qDebug() << "Files to be uploaded:" << toUpload;
            qInfo() << "Number of files to upload:" << toUpload.size();
        }

        // Now generate stuff using additional QSqlQueries
        const QString pid(CharcoalDbHelpers::getEntityName(m_connectionName, entityId));
        const QString action(CharcoalDbHelpers::getEventType(m_connectionName, typeId));

        const QJsonDocument doc(
            {
                { Tags::pid, pid },
                { Tags::action, action },
                { Tags::timestamp, timestamp },
                { Tags::location, location },
                { Tags::properties, dbMapToWebObject(propertiesObject) }
            });

        QSharedPointer<BaseRequest> request;

        if (toUpload.isEmpty()) {
            request = QSharedPointer<BaseRequest>::create(
                "/entities/new/",
                BaseRequest::Type::Post
                );

            request->setDocument(doc);
        } else {
            auto multi = QSharedPointer<MultiPartRequest>::create(
                "/entities/new/",
                MultiPartRequest::Type::Post
                );

            const auto docObject = doc.object();
            for (const QString &key : docObject.keys()) {
                const QJsonValue value(docObject.value(key));
                if (value.isObject()) {
                    multi->addPart(key, QJsonDocument(value.toObject()).toJson(
                                            QJsonDocument::JsonFormat::Compact));
                } else {
                    multi->addPart(key, value.toVariant().toString());
                }
            }

            for (const QString &path : qAsConst(toUpload)) {
                const QFileInfo info(m_picturesManager->cachePath() + "/" + path);
                if (info.fileName().startsWith(Tags::document)) {
                    multi->addPart(Tags::webDocuments, info);
                } else  {
                    multi->addPart(Tags::webReceipts, info);
                }
            }

            request = multi;
        }

        // We send one event right away, and queue the rest
        if (isLoggedIn && i == 0) {
            qDebug().noquote() << "Sending event!" << doc;
            request->setToken(m_sessionManager->token());
            m_sessionManager->sendRequest(request, this,
                                          &EventsSender::webErrorHandler,
                                          &EventsSender::webReplyHandler);
        } else {
            qDebug().noquote() << "Enqueuing event!" << doc;
            m_queuedRequests.append(request);
        }

        // We quit after one loop - next even will be sent when current one is finished.
        m_hasPendingEvent = true;
        return;
    }
}
