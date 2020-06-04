#include "restsessionmanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QJsonObject>
#include <QJsonArray>
#include <QGeoCoordinate>

#include "common/tags.h"
#include "common/globals.h"
#include "rest/authrequest.h"
#include "rest/entityrequest.h"
#include "rest/additionaldatarequest.h"

RestSessionManager::RestSessionManager(QObject *parent)
    : AbstractSessionManager(parent)
{}

void RestSessionManager::ping()
{
    const auto errorHandler = [this](const QString &, const QNetworkReply::NetworkError &code) {
        emit pingError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &) {
        emit pingSuccess();
    };
    sendRequest(QSharedPointer<AuthRequest>::create(), errorHandler, replyHandler, true);
}

void RestSessionManager::login(const QString &login, const QString &password)
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

void RestSessionManager::getAdditionalData()
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

void RestSessionManager::getEntitiesInfo(const QDateTime &from, const QDateTime &to)
{
    const auto errorHandler = [this](const QString &, const QNetworkReply::NetworkError &code) {
        emit entitiesLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit entitiesInfoLoaded(reply.object().value(Tags::results).toArray());
    };

    sendRequest(QSharedPointer<EntityRequest>::create(from, to), errorHandler, replyHandler);
}

void RestSessionManager::getEntitiesInfo(int limit, int offset, const QDateTime &from, const QDateTime &to)
{
    const auto errorHandler = [this](const QString &, const QNetworkReply::NetworkError &code) {
        emit entitiesLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit entitiesInfoLoaded(reply.object().value(Tags::results).toArray());
    };

    sendRequest(QSharedPointer<EntityRequest>::create(limit, offset, from, to), errorHandler, replyHandler);
}

void RestSessionManager::getEntitiesInfo(int limit, int offset, const QString &keyword,
                                     const QSet<Enums::PackageType> &filteredPackages, int cooperativeId)
{
    const auto errorHandler = [this](const QString &, const QNetworkReply::NetworkError &code) {
        emit entitiesLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit entitiesInfoLoaded(reply.object().value(Tags::results).toArray());
    };

    sendRequest(QSharedPointer<EntityRequest>::create(limit, offset, keyword, filteredPackages, cooperativeId), errorHandler, replyHandler);
}

void RestSessionManager::getLastActionEntitiesInfo(const Enums::SupplyChainAction &lastAction)
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

void RestSessionManager::getEntities(const QStringList &packageIds)
{
    const auto errorHandler = [this](const QString &, const QNetworkReply::NetworkError &code) {
        emit entitiesLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit entitiesLoaded(reply.object().value(Tags::results).toArray());
    };
    sendRequest(QSharedPointer<EntityRequest>::create(packageIds), errorHandler, replyHandler);
}

void RestSessionManager::postNewEntity(const QString &packageId, const Enums::SupplyChainAction &action,
                                   const QGeoCoordinate &coordinate, const QDateTime &timestamp, const QVariantMap &properties)
{
    const auto errorHandler = [this, packageId, action](const QString &, const QNetworkReply::NetworkError &code) {
        emit entitySaveError(packageId, {}, action, code);
    };
    const auto replyHandler = [this, action](const QJsonDocument &reply) {
        emit entitySaved(reply.object().value(Tags::pid).toString(), {}, action);
    };

    if (checkValidToken()) {
        sendRequest(QSharedPointer<EntityRequest>::create(m_token, packageId, EntityRequest::EntityData{ action, coordinate, timestamp, properties }),
                    errorHandler, replyHandler);
    }
}

void RestSessionManager::postNewEntity(const QString &packageId, const QByteArray &codeData, const Enums::SupplyChainAction &action,
                                   const QGeoCoordinate &coordinate, const QDateTime &timestamp, const QVariantMap &properties)
{
    const auto errorHandler = [this, packageId, action](const QString &, const QNetworkReply::NetworkError &code) {
        emit entitySaveError(packageId, {}, action, code);
    };
    const auto replyHandler = [this, action](const QJsonDocument &reply) {
        emit entitySaved(reply.object().value(Tags::pid).toString(), {}, action);
    };

    if (checkValidToken()) {
        sendRequest(QSharedPointer<EntityRequest>::create(m_token, packageId, codeData, EntityRequest::EntityData{ action, coordinate, timestamp, properties }),
                    errorHandler, replyHandler);
    }
}

void RestSessionManager::postNewEntity(const QByteArray &codeData, const Enums::SupplyChainAction &action,
                                   const QGeoCoordinate &coordinate, const QDateTime &timestamp, const QVariantMap &properties)
{
    const auto errorHandler = [this, codeData, action](const QString &, const QNetworkReply::NetworkError &code) {
        emit entitySaveError({}, codeData, action, code);
    };
    const auto replyHandler = [this, codeData, action](const QJsonDocument &reply) {
        emit entitySaved(reply.object().value(Tags::pid).toString(), codeData, action);
    };

    if (checkValidToken()) {
        sendRequest(QSharedPointer<EntityRequest>::create(m_token, codeData, EntityRequest::EntityData{ action, coordinate, timestamp, properties }),
                    errorHandler, replyHandler);
    }
}

void RestSessionManager::sendRequest(const QSharedPointer<BaseRequest> &request,
                                 const std::function<void (const QString &, const QNetworkReply::NetworkError &)> &errorHandler,
                                 const std::function<void (const QJsonDocument &)> &replyHandler,
                                 bool force)
{
    if (!enabled() && !force) {
        return;
    }

    connect(request.data(), &BaseRequest::replyError,
            this, [errorHandler](const QString &msgs, const int errorCode) { errorHandler(msgs, static_cast<QNetworkReply::NetworkError>(errorCode)); });
    connect(request.data(), &BaseRequest::requestFinished, this, replyHandler);

    sendRequest(request);
}

void RestSessionManager::sendRequest(const QSharedPointer<BaseRequest> &request)
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
