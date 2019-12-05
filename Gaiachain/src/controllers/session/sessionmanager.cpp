#include "sessionmanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QJsonObject>
#include <QJsonArray>

#include "../../common/tags.h"
#include "../../common/globals.h"
#include "../../rest/authrequest.h"
#include "../../rest/entityrequest.h"
#include "../../rest/additionaldatarequest.h"

SessionManager::SessionManager(QObject *parent)
    : AbstractSessionManager(parent)
{}

void SessionManager::ping()
{
    const auto errorHandler = [this](const QString &, const QNetworkReply::NetworkError &code) {
        emit pingError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &) {
        emit pingSuccess();
    };
    sendRequest(QSharedPointer<AuthRequest>::create(), errorHandler, replyHandler);
}

void SessionManager::login(const QString &login, const QString &password)
{
    emit loginAttempt(login, password);

    const auto errorHandler = [this, login](const QString &, const QNetworkReply::NetworkError &code) {
        emit loginError(login, code);
    };
    const auto replyHandler = [this, login](const QJsonDocument &reply) {
        if (reply.isObject()) {
            emit loginFinished(login, reply.object());
        } else {
            emit loginError(login, QNetworkReply::NetworkError::UnknownContentError);
        }
    };
    sendRequest(QSharedPointer<AuthRequest>::create(login, password), errorHandler, replyHandler);
}

void SessionManager::getAdditionalData()
{
    const auto errorHandler = [this](const QString &, const QNetworkReply::NetworkError &code) {
        emit additionalDataLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit additionalDataLoaded(reply.object());
    };
    sendRequest(QSharedPointer<AdditionalDataRequest>::create(AdditionalDataRequest::DataType::All),
                errorHandler, replyHandler);
}

void SessionManager::getProducers()
{
    const auto errorHandler = [this](const QString &, const QNetworkReply::NetworkError &code) {
        emit additionalDataLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit additionalDataLoaded(QJsonObject{ { Tags::producers, reply.object().value(Tags::results) } });
    };
    sendRequest(QSharedPointer<AdditionalDataRequest>::create(AdditionalDataRequest::DataType::Producers),
                errorHandler, replyHandler);
}

void SessionManager::getCompanies()
{
    const auto errorHandler = [this](const QString &, const QNetworkReply::NetworkError &code) {
        emit additionalDataLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit additionalDataLoaded(QJsonObject{ { Tags::companies, reply.object().value(Tags::results) } });
    };
    sendRequest(QSharedPointer<AdditionalDataRequest>::create(AdditionalDataRequest::DataType::Companies),
                errorHandler, replyHandler);
}

void SessionManager::getDestinations()
{
    const auto errorHandler = [this](const QString &, const QNetworkReply::NetworkError &code) {
        emit additionalDataLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit additionalDataLoaded(QJsonObject{ { Tags::destinations, reply.object().value(Tags::results) } });
    };
    sendRequest(QSharedPointer<AdditionalDataRequest>::create(AdditionalDataRequest::DataType::Destinations),
                errorHandler, replyHandler);
}

void SessionManager::getEntitiesInfo(int limit, const QDateTime &to, const QString &keyword)
{
    const auto errorHandler = [this](const QString &, const QNetworkReply::NetworkError &code) {
        emit entitiesLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit entitiesInfoLoaded(reply.object().value(Tags::results).toArray());
    };

    auto toDate = to.isNull() ? QDateTime::currentDateTime() : to;
    sendRequest(QSharedPointer<EntityRequest>::create(limit, toDate, keyword), errorHandler, replyHandler);
}

void SessionManager::getEntitiesInfo(const QDateTime &from, const QDateTime &to, const QString &keyword)
{
    const auto errorHandler = [this](const QString &, const QNetworkReply::NetworkError &code) {
        emit entitiesLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit entitiesInfoLoaded(reply.object().value(Tags::results).toArray());
    };

    sendRequest(QSharedPointer<EntityRequest>::create(from, to, keyword), errorHandler, replyHandler);
}

void SessionManager::getLastActionEntitiesInfo(const Enums::SupplyChainAction &lastAction)
{
    const auto errorHandler = [this](const QString &, const QNetworkReply::NetworkError &code) {
        emit entitiesLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit entitiesInfoLoaded(reply.object().value(Tags::results).toArray());
    };

    if (checkValidToken()) {
        sendRequest(QSharedPointer<EntityRequest>::create(m_token, lastAction), errorHandler, replyHandler);
    }
}

void SessionManager::getEntities(const QStringList &packageIds)
{
    const auto errorHandler = [this](const QString &, const QNetworkReply::NetworkError &code) {
        emit entitiesLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit entitiesLoaded(reply.object().value(Tags::results).toArray());
    };
    sendRequest(QSharedPointer<EntityRequest>::create(packageIds), errorHandler, replyHandler);
}

void SessionManager::postNewEntity(const QString &packageId, const Enums::SupplyChainAction &action,
                                   const QDateTime &timestamp, const QVariantMap &properties)
{
    const auto errorHandler = [this, action](const QString &, const QNetworkReply::NetworkError &code) {
        emit entitySaveError({}, action, code);
    };
    const auto replyHandler = [this, action](const QJsonDocument &reply) {
        emit entitySaved(reply.object().value(Tags::pid).toString(), action);
    };

    if (checkValidToken()) {
        sendRequest(QSharedPointer<EntityRequest>::create(m_token, packageId, EntityRequest::EntityData{ action, timestamp, properties }),
                    errorHandler, replyHandler);
    }
}

void SessionManager::postNewEntity(const QString &packageId, const QByteArray &codeData, const Enums::SupplyChainAction &action,
                                   const QDateTime &timestamp, const QVariantMap &properties)
{
    const auto errorHandler = [this, action](const QString &, const QNetworkReply::NetworkError &code) {
        emit entitySaveError({}, action, code);
    };
    const auto replyHandler = [this, action](const QJsonDocument &reply) {
        emit entitySaved(reply.object().value(Tags::pid).toString(), action);
    };

    if (checkValidToken()) {
        sendRequest(QSharedPointer<EntityRequest>::create(m_token, packageId, codeData, EntityRequest::EntityData{ action, timestamp, properties }),
                    errorHandler, replyHandler);
    }
}

void SessionManager::postNewEntity(const QByteArray &codeData, const Enums::SupplyChainAction &action,
                                   const QDateTime &timestamp, const QVariantMap &properties)
{
    const auto errorHandler = [this, action](const QString &, const QNetworkReply::NetworkError &code) {
        emit entitySaveError({}, action, code);
    };
    const auto replyHandler = [this, action](const QJsonDocument &reply) {
        emit entitySaved(reply.object().value(Tags::pid).toString(), action);
    };

    if (checkValidToken()) {
        sendRequest(QSharedPointer<EntityRequest>::create(m_token, codeData, EntityRequest::EntityData{ action, timestamp, properties }),
                    errorHandler, replyHandler);
    }
}

void SessionManager::getUnusedLotIds()
{
    const auto errorHandler = [this](const QString &, const QNetworkReply::NetworkError &code) {
        emit unusedLotIdsLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit unusedLotIdsLoaded(reply.object().value(Tags::results).toArray());
    };

    if (checkValidToken()) {
        sendRequest(QSharedPointer<EntityRequest>::create(m_token, Enums::PackageType::Lot), errorHandler, replyHandler);
    }
}

void SessionManager::postUnusedLotId()
{
    const auto errorHandler = [this](const QString &, const QNetworkReply::NetworkError &code) {
        emit unusedLotIdCreateError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit unusedLotIdCreated(reply.object().value(Tags::pid).toString());
    };

    if (checkValidToken()) {
        sendRequest(QSharedPointer<EntityRequest>::create(m_token, Enums::PackageType::Lot, true), errorHandler, replyHandler);
    }
}

void SessionManager::sendRequest(const QSharedPointer<BaseRequest> &request,
                                 const std::function<void (const QString &, const QNetworkReply::NetworkError &)> &errorHandler,
                                 const std::function<void (const QJsonDocument &)> &replyHandler)
{
    connect(request.data(), &BaseRequest::replyError,
            this, [errorHandler](const QString &msgs, const int errorCode) { errorHandler(msgs, static_cast<QNetworkReply::NetworkError>(errorCode)); });
    connect(request.data(), &BaseRequest::requestFinished, this, replyHandler);

    sendRequest(request);
}

void SessionManager::sendRequest(const QSharedPointer<BaseRequest> &request)
{
    connect(request.data(), &BaseRequest::requestFinished,
            this, [this](const QJsonDocument &) {
        updateConnectionStateAfterRequest();
        processFinished();
    });
    connect(request.data(), &BaseRequest::replyError,
            this, [this](const QString &, const int errorCode) {
        updateConnectionStateAfterRequest(QNetworkReply::NetworkError(errorCode));
        processFinished();
    });

    processStarted();
    updateConnectionStateBeforeRequest();

    m_client.send(request);
}
