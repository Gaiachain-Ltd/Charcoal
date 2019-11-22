#include "sessionmanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QJsonObject>
#include <QJsonArray>

#include "../../common/tags.h"
#include "../../common/globals.h"
#include "../../helpers/utility.h"
#include "../../rest/authrequest.h"
#include "../../rest/entityrequest.h"
#include "../../rest/relationrequest.h"
#include "../../rest/additionaldatarequest.h"

SessionManager::SessionManager(QObject *parent)
    : AbstractSessionManager(parent)
{}

void SessionManager::ping()
{
    const auto errorHandler = [this](const QString &, const int &code) {
        emit pingError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &) {
        emit pingSuccess();
    };
    sendRequest(QSharedPointer<AuthRequest>::create(), errorHandler, replyHandler);
}

void SessionManager::login(const QString &email, const QString &password)
{
    const auto errorHandler = [this](const QString &, const int &code) {
        emit loginError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit loginFinished(reply);
    };
    sendRequest(QSharedPointer<AuthRequest>::create(email, password), errorHandler, replyHandler);
}

void SessionManager::getAdditionalData()
{
    const auto errorHandler = [this](const QString &, const int &code) {
        emit additionalDataLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit additionalDataLoaded(reply.object());
    };
    sendRequest(QSharedPointer<AdditionalDataRequest>::create(AdditionalDataRequest::DataType::All),
                errorHandler, replyHandler);
}

void SessionManager::getRelations(const QString &packageId)
{
    const auto errorHandler = [this](const QString &, const int &code) {
        emit relationsLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit relationsLoaded(reply.object().value(Tags::ids).toArray());
    };
    sendRequest(QSharedPointer<RelationRequest>::create(packageId), errorHandler, replyHandler);
}

void SessionManager::getRelations(const QStringList &packageIds)
{
    const auto errorHandler = [this](const QString &, const int &code) {
        emit relationsLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit relationsLoaded(reply.object().value(Tags::relations).toArray());
    };
    sendRequest(QSharedPointer<RelationRequest>::create(packageIds), errorHandler, replyHandler);
}

void SessionManager::addRelation(const QString &packageId, const QStringList &relatedIds)
{
    const auto errorHandler = [this](const QString &, const int &code) {
        emit relationsLoadError(code);
    };
    const auto replyHandler = [this, packageId](const QJsonDocument &) {
        emit relationsSaved(packageId);
    };

    if (checkValidToken()) {
        sendRequest(QSharedPointer<RelationRequest>::create(m_token, packageId, relatedIds), errorHandler, replyHandler);
    }
}

void SessionManager::getEntitiesInfo(int count, const QDateTime &from, const QString &keyword)
{
    const auto errorHandler = [this](const QString &, const int &code) {
        emit entitiesLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit entitiesLoaded(reply.object().value(Tags::entities).toArray());
    };

    auto fromDate = from.isNull() ? QDateTime::currentDateTime() : from;
    sendRequest(QSharedPointer<EntityRequest>::create(count, fromDate, keyword), errorHandler, replyHandler);
}

void SessionManager::getEntitiesInfo(const QDateTime &from, const QDateTime &to, const QString &keyword)
{
    const auto errorHandler = [this](const QString &, const int &code) {
        emit entitiesLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit entitiesLoaded(reply.object().value(Tags::entities).toArray());
    };

    sendRequest(QSharedPointer<EntityRequest>::create(from, to, keyword), errorHandler, replyHandler);
}

void SessionManager::getEntities(const QStringList &packageIds)
{
    const auto errorHandler = [this](const QString &, const int &code) {
        emit entitiesLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit entitiesLoaded(reply.object().value(Tags::entities).toArray());
    };
    sendRequest(QSharedPointer<EntityRequest>::create(packageIds), errorHandler, replyHandler);
}

void SessionManager::getEntity(const QString &packageId)
{
    const auto errorHandler = [this](const QString &, const int &code) {
        emit entitiesLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit entitiesLoaded(reply.object().value(Tags::entities).toArray());
    };
    sendRequest(QSharedPointer<EntityRequest>::create(QStringList{ packageId, }), errorHandler, replyHandler);
}

void SessionManager::getEntityId(const QByteArray &codeData)
{
    const auto errorHandler = [this](const QString &, const int &code) {
        emit entityIdLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit entityIdLoaded(reply.object().value(Tags::id).toString());
    };
    sendRequest(QSharedPointer<EntityRequest>::create(codeData), errorHandler, replyHandler);
}

void SessionManager::putEntityAction(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties, const QByteArray &codeData)
{
    const auto errorHandler = [this](const QString &, const int &code) {
        emit entitySaveError(code);
    };
    const auto replyHandler = [this, packageId, action](const QJsonDocument &) {
        emit entitySaved(packageId, action);
    };

    if (checkValidToken()) {
        sendRequest(QSharedPointer<EntityRequest>::create(m_token, packageId, EntityRequest::EntityData{ action, timestamp, properties }, codeData),
                    errorHandler, replyHandler);
    }
}

void SessionManager::putEntityAction(const QByteArray &codeData, const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties)
{
    const auto errorHandler = [this](const QString &, const int &code) {
        emit entitySaveError(code);
    };
    const auto replyHandler = [this, action](const QJsonDocument &reply) {
        emit entitySaved(reply.object().value(Tags::id).toString(), action);
    };

    if (checkValidToken()) {
        sendRequest(QSharedPointer<EntityRequest>::create(m_token, codeData, EntityRequest::EntityData{ action, timestamp, properties }, false),
                    errorHandler, replyHandler);
    }
}

void SessionManager::postNewEntity(const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties, const QByteArray &codeData)
{
    const auto errorHandler = [this](const QString &, const int &code) {
        emit entitySaveError(code);
    };
    const auto replyHandler = [this, action](const QJsonDocument &reply) {
        emit entitySaved(reply.object().value(Tags::id).toString(), action);
    };

    if (checkValidToken()) {
        sendRequest(QSharedPointer<EntityRequest>::create(m_token, codeData, EntityRequest::EntityData{ action, timestamp, properties }, true),
                    errorHandler, replyHandler);
    }
}

void SessionManager::getCreatedHarvestIds()
{
    const auto errorHandler = [this](const QString &, const int &code) {
        emit createdHarvestIdsLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit createdHarvestIdsLoaded(reply.object().value(Tags::ids).toArray());
    };

    if (checkValidToken()) {
        sendRequest(QSharedPointer<EntityRequest>::create(m_token, Enums::PackageType::Harvest), errorHandler, replyHandler);
    }
}

void SessionManager::getUnusedLotIds()
{
    const auto errorHandler = [this](const QString &, const int &code) {
        emit unusedLotIdsLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit unusedLotIdsLoaded(reply.object().value(Tags::ids).toArray());
    };

    if (checkValidToken()) {
        sendRequest(QSharedPointer<EntityRequest>::create(m_token, Enums::PackageType::Lot), errorHandler, replyHandler);
    }
}

void SessionManager::postUnusedLotId()
{
    const auto errorHandler = [this](const QString &, const int &code) {
        emit unusedLotIdCreateError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit unusedLotIdCreated(reply.object().value(Tags::id).toString());
    };

    if (checkValidToken()) {
        sendRequest(QSharedPointer<EntityRequest>::create(m_token, Enums::PackageType::Lot, true), errorHandler, replyHandler);
    }
}

void SessionManager::sendRequest(const QSharedPointer<BaseRequest> &request,
                                 const std::function<void (const QString &, const int &)> &errorHandler,
                                 const std::function<void (const QJsonDocument &)> &replyHandler)
{
    connect(request.data(), &BaseRequest::replyError, this, errorHandler);
    connect(request.data(), &BaseRequest::requestFinished, this, replyHandler);

    sendRequest(request);
}

void SessionManager::sendRequest(const QSharedPointer<BaseRequest> &request)
{
    connect(request.data(), &BaseRequest::requestFinished,
            this, [this](const QJsonDocument &) {
        updateConnectionStateAfterRequest();
    });
    connect(request.data(), &BaseRequest::replyError,
            this, [this](const QString &, const int errorCode) {
        updateConnectionStateAfterRequest(QNetworkReply::NetworkError(errorCode));
    });

    updateConnectionStateBeforeRequest();
    m_client.send(request);
}
