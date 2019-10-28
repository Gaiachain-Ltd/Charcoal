#include "sessionmanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QJsonObject>
#include <QJsonArray>

#include "../../common/tags.h"
#include "../../common/globals.h"
#include "../../helpers/utility.h"
#include "../../rest/loginrequest.h"
#include "../../rest/entityrequest.h"

SessionManager::SessionManager(QObject *parent)
    : AbstractSessionManager(parent)
{}

void SessionManager::login(const QString &email, const QString &password)
{
    auto request = QSharedPointer<LoginRequest>::create(email, password);

    auto finishLambda = [&](const QJsonDocument &reply) {
        emit loginFinished(reply);
    };

    auto errorLambda = [&](const QString &, const int code) {
        emit loginError(code);
    };

    connect(request.data(), &BaseRequest::requestFinished, finishLambda);
    connect(request.data(), &BaseRequest::replyError, errorLambda);

    sendRequest(request);
}

void SessionManager::getAdditionalData()
{
    // TODO implement
}

void SessionManager::getRelations(const QString &id)
{
    Q_UNUSED(id)
    // TODO implement
}

void SessionManager::addRelation(const QString &id, const QStringList &ids)
{
    Q_UNUSED(id)
    Q_UNUSED(ids)
    // TODO implement
}

void SessionManager::getEntitiesInfo(int count, const QDateTime &from)
{
    Q_UNUSED(count)
    Q_UNUSED(from)
    // TODO implement
}

void SessionManager::getEntitiesInfo(const QDateTime &to, const QDateTime &from)
{
    Q_UNUSED(to)
    Q_UNUSED(from)
    // TODO implement
}

void SessionManager::getEntities(const QStringList &ids)
{
    Q_UNUSED(ids)
    // TODO implement
}

void SessionManager::getEntity(const QString &id)
{
    auto request = QSharedPointer<EntityRequest>::create(m_token, id);

    auto errorLambda = [&](const QString &, const int code) {
        emit entityLoadError(code);
    };

    auto finishLambda = [&](const QJsonDocument &reply) {
        emit entitiesLoaded(reply.array());
    };

    connect(request.data(), &BaseRequest::requestFinished, finishLambda);
    connect(request.data(), &BaseRequest::replyError, errorLambda);

    sendRequest(request);
}

void SessionManager::getEntitId(const QByteArray &codeData)
{
    Q_UNUSED(codeData)
    // TODO implement
}

void SessionManager::putEntityAction(const QString &id, const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties, const QByteArray &codeData)
{
    Q_UNUSED(timestamp)
    Q_UNUSED(codeData)
    // TODO update

    if (checkValidToken()) {
        auto request = QSharedPointer<EntityRequest>::create(m_token, id, action, properties);

        auto errorLambda = [&, id](const QString &, const int) {
            emit entitySaveResult(id, false);
        };

        auto finishLambda = [&, id](const QJsonDocument &) {
            emit entitySaveResult(id, true);
        };

        connect(request.data(), &BaseRequest::requestFinished, finishLambda);
        connect(request.data(), &BaseRequest::replyError, errorLambda);

        sendRequest(request);
    }
}

void SessionManager::putEntity(const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties, const QByteArray &codeData)
{
    Q_UNUSED(action)
    Q_UNUSED(timestamp)
    Q_UNUSED(properties)
    Q_UNUSED(codeData)
    // TODO implement
}

void SessionManager::getUnusedLotIds()
{
    // TODO implement
}

void SessionManager::createUnusedLotId()
{
    // TODO implement
}

void SessionManager::getAllRelations()
{
    // TODO implement
}

void SessionManager::getEntities(const QJsonArray &ids)
{
    auto request = QSharedPointer<EntityRequest>::create(m_token, ids);

    auto errorLambda = [&](const QString &, const int code) {
        emit entityLoadError(code);
    };

    auto finishLambda = [&](const QJsonDocument &reply) {
        QJsonArray array;
        const QJsonObject &obj = reply.object();
        QJsonObject::const_iterator it = obj.constBegin();
        while (it != obj.constEnd()) {
            array.push_back(*it);
            ++it;
        }
        emit entitiesLoaded(array);
    };

    connect(request.data(), &BaseRequest::requestFinished, finishLambda);
    connect(request.data(), &BaseRequest::replyError, errorLambda);

    sendRequest(request);
}

void SessionManager::getAllEntities()
{
    auto request = QSharedPointer<EntityRequest>::create(m_token);

    auto errorLambda = [&](const QString &, const int code) {
        emit entityLoadError(code);
    };

    auto finishLambda = [&](const QJsonDocument &reply) {
        const QJsonArray &array = reply.array();
        QJsonArray::const_iterator it = array.constBegin();
        QJsonArray batchArray;
        while(it != array.constEnd()) {
            batchArray.push_back(*it);
            if (batchArray.size() > MAX_BATCH_SIZE) {
                this->getEntities(batchArray);
                batchArray = QJsonArray();
            }
            ++it;
        }
        if (batchArray.size() > 0)
            this->getEntities(batchArray);
    };

    connect(request.data(), &BaseRequest::requestFinished, finishLambda);
    connect(request.data(), &BaseRequest::replyError, errorLambda);

    sendRequest(request);
}

void SessionManager::sendRequest(const QSharedPointer<BaseRequest> &request)
{
    connect(request.data(), &BaseRequest::requestFinished,
            this, [this](const QJsonDocument &) { updateConnectionStateAfterRequest(); });
    connect(request.data(), &BaseRequest::replyError,
            this, [this](const QString &, const int errorCode) { updateConnectionStateAfterRequest(QNetworkReply::NetworkError(errorCode)); });

    updateConnectionStateBeforeRequest();
    m_client.send(request);
}
