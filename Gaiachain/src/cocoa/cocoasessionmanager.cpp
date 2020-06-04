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
