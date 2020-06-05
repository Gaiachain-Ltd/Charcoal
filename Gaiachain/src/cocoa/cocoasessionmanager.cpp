#include "cocoasessionmanager.h"

#include "common/tags.h"
#include "rest/additionaldatarequest.h"
#include "rest/entityrequest.h"

#include <QJsonObject>
#include <QJsonArray>

CocoaSessionManager::CocoaSessionManager(QObject *parent)
    : RestSessionManager(parent)
{
}

void CocoaSessionManager::getProducers()
{
    const auto errorHandler = [this](const QString &, const QNetworkReply::NetworkError &code) {
        emit additionalDataLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit additionalDataLoaded(QJsonObject{ { Tags::producers, reply.object().value(Tags::results).toArray() } });
    };
    sendRequest(QSharedPointer<AdditionalDataRequest>::create(AdditionalDataRequest::DataType::Producers),
                errorHandler, replyHandler);
}

void CocoaSessionManager::getCompanies()
{
    const auto errorHandler = [this](const QString &, const QNetworkReply::NetworkError &code) {
        emit additionalDataLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit additionalDataLoaded(QJsonObject{ { Tags::companies, reply.object().value(Tags::results).toArray() } });
    };
    sendRequest(QSharedPointer<AdditionalDataRequest>::create(AdditionalDataRequest::DataType::Companies),
                errorHandler, replyHandler);
}

void CocoaSessionManager::getDestinations()
{
    const auto errorHandler = [this](const QString &, const QNetworkReply::NetworkError &code) {
        emit additionalDataLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit additionalDataLoaded(QJsonObject{ { Tags::destinations, reply.object().value(Tags::results).toArray() } });
    };
    sendRequest(QSharedPointer<AdditionalDataRequest>::create(AdditionalDataRequest::DataType::Destinations),
                errorHandler, replyHandler);
}

void CocoaSessionManager::getUnusedLotIds()
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

void CocoaSessionManager::postUnusedLotId()
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

void CocoaSessionManager::getAdditionalData()
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

void CocoaSessionManager::getEntitiesInfo(const QDateTime &from, const QDateTime &to)
{
    const auto errorHandler = [this](const QString &, const QNetworkReply::NetworkError &code) {
        emit entitiesLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit entitiesInfoLoaded(reply.object().value(Tags::results).toArray());
    };

    sendRequest(QSharedPointer<EntityRequest>::create(from, to), errorHandler, replyHandler);
}

void CocoaSessionManager::getEntitiesInfo(int limit, int offset,
                                          const QDateTime &from,
                                          const QDateTime &to)
{
    const auto errorHandler = [this](const QString &, const QNetworkReply::NetworkError &code) {
        emit entitiesLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit entitiesInfoLoaded(reply.object().value(Tags::results).toArray());
    };

    sendRequest(QSharedPointer<EntityRequest>::create(limit, offset, from, to), errorHandler, replyHandler);
}

void CocoaSessionManager::getEntitiesInfo(int limit, int offset,
                                          const QString &keyword,
                                          const QSet<Enums::PackageType> &filteredPackages,
                                          int cooperativeId)
{
    const auto errorHandler = [this](const QString &, const QNetworkReply::NetworkError &code) {
        emit entitiesLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit entitiesInfoLoaded(reply.object().value(Tags::results).toArray());
    };

    sendRequest(QSharedPointer<EntityRequest>::create(limit, offset, keyword, filteredPackages, cooperativeId), errorHandler, replyHandler);
}

void CocoaSessionManager::getLastActionEntitiesInfo(
    const Enums::SupplyChainAction &lastAction)
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

void CocoaSessionManager::getEntities(const QStringList &packageIds)
{
    const auto errorHandler = [this](const QString &, const QNetworkReply::NetworkError &code) {
        emit entitiesLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit entitiesLoaded(reply.object().value(Tags::results).toArray());
    };
    sendRequest(QSharedPointer<EntityRequest>::create(packageIds), errorHandler, replyHandler);
}

void CocoaSessionManager::postNewEntity(const QString &packageId,
                                        const Enums::SupplyChainAction &action,
                                        const QGeoCoordinate &coordinate,
                                        const QDateTime &timestamp,
                                        const QVariantMap &properties)
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

void CocoaSessionManager::postNewEntity(const QString &packageId,
                                        const QByteArray &codeData,
                                        const Enums::SupplyChainAction &action,
                                        const QGeoCoordinate &coordinate,
                                        const QDateTime &timestamp,
                                        const QVariantMap &properties)
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

void CocoaSessionManager::postNewEntity(const QByteArray &codeData,
                                        const Enums::SupplyChainAction &action,
                                        const QGeoCoordinate &coordinate,
                                        const QDateTime &timestamp,
                                        const QVariantMap &properties)
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
