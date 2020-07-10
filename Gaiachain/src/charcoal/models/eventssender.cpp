#include "eventssender.h"

#include "common/tags.h"
#include "common/logs.h"
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
               "FROM Events WHERE isCommitted=0");

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

        // First, get query data. Subsequent calls to any QSqlQuery instances
        // might break the main query!
        const qint64 timestamp =
            static_cast<qint64>(query().value("date").toLongLong());
        const QString properties(query().value(Tags::properties).toString());

        const int entityId(query().value("entityId").toInt());
        const int typeId(query().value("typeId").toInt());

        const QJsonObject location({
            { "latitude", query().value("locationLatitude").toDouble() },
            { "longitude", query().value("locationLongitude").toDouble() }
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
                { Tags::properties, dbMapToWebObject(propertiesObject, entityId) }
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

void EventsSender::onFinalizePackage(const int webId)
{
    const auto request = QSharedPointer<BaseRequest>::create(
        QString("/entities/packages/%1/finalize_supply_chain/").arg(webId),
        BaseRequest::Type::Post);

    if (m_userManager->isLoggedIn()) {
        request->setToken(m_sessionManager->token());
        m_sessionManager->sendRequest(request, this,
                                      &EventsSender::webErrorHandler,
                                      &EventsSender::finalizationReplyHandler);
    } else {
        qDebug() << "Enqueing request";
        m_queuedRequests.append(request);
    }
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
}

void EventsSender::webReplyHandler(const QJsonDocument &reply)
{
    qDebug() << "Request success!" << reply;

    const QString pid(reply.object().value(Tags::pid).toString());
    const qint64 entityWebId(reply.object().value(Tags::webEventId).toInt());
    const qint64 timestamp(reply.object().value(Tags::eventTimestamp).toVariant().toLongLong());
    const QString eventId(findEventByTimestamp(timestamp));

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));

    query.prepare("UPDATE Events "
                  "SET isCommitted=1, parentWebId=:entityWebId "
                  "WHERE date=:timestamp");
    query.bindValue(":entityWebId", entityWebId);
    query.bindValue(":timestamp", timestamp);

    if (query.exec()) {
        if (updateEntityWebId(entityWebId, eventId) == false) {
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
                   << timestamp << Tags::pid << pid << "eid" << entityWebId;
        emit error(errorString);
    }
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
        const QDateTime timestamp(QDateTime::fromString(justDateTime, "yyyy-MM-ddTHHmmss"));
        //path = QString("%1/%2/%3").arg(path, timestamp.toString("yyyy/MM/dd"), path);
        adjusted.append(timestamp.toString("yyyy/MM/dd"));
        adjusted.append("/");
        adjusted.append(path);
    }

    qDebug() << "Fetch photo:" << adjusted;
    const auto request = QSharedPointer<ImageRequest>::create(
        adjusted, m_picturesManager->cachePath());

    if (m_userManager->isLoggedIn()) {
        m_sessionManager->sendRequest(request);
    } else {
        qDebug() << "Enqueing request";
        m_queuedRequests.append(request);
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
QJsonObject EventsSender::dbMapToWebObject(QJsonObject object,
                                           const int entityId) const
{
    if (object.value(Tags::webPlotId).isNull()) {
        // Plot ID from web was unknown when DB entry was created.
        // We need to correct it
        const int webPlotId(CharcoalDbHelpers::getWebPackageId(
            m_connectionName, entityId));
        if (webPlotId == -1) {
            const QLatin1String errorString = QLatin1String("Failed to find Plot ID (Web) for plot");
            qDebug() << errorString << object;
            emit error(errorString);
        } else {
            object.insert(Tags::webPlotId, webPlotId);
        }
    }

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
