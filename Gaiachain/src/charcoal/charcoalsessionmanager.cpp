#include "charcoalsessionmanager.h"

#include "common/tags.h"
#include "rest/additionaldatarequest.h"

#include <QJsonObject>
#include <QJsonArray>

CharcoalSessionManager::CharcoalSessionManager(QObject *parent)
    : RestSessionManager(parent)
{
}

void CharcoalSessionManager::getDestinations()
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

void CharcoalSessionManager::getOvenTypes()
{
    const auto errorHandler = [this](const QString &, const QNetworkReply::NetworkError &code) {
        emit additionalDataLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit additionalDataLoaded(QJsonObject{ { Tags::ovenTypes, reply.object().value(Tags::results).toArray() } });
    };
    sendRequest(QSharedPointer<AdditionalDataRequest>::create(AdditionalDataRequest::DataType::OvenTypes),
                errorHandler, replyHandler);
}

void CharcoalSessionManager::getParcels()
{
    const auto errorHandler = [this](const QString &, const QNetworkReply::NetworkError &code) {
        emit additionalDataLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit additionalDataLoaded(QJsonObject{ { Tags::parcels, reply.object().value(Tags::results).toArray() } });
    };
    sendRequest(QSharedPointer<AdditionalDataRequest>::create(AdditionalDataRequest::DataType::Parcels),
                errorHandler, replyHandler);
}

void CharcoalSessionManager::getTreeSpecies()
{
    const auto errorHandler = [this](const QString &, const QNetworkReply::NetworkError &code) {
        emit additionalDataLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit additionalDataLoaded(QJsonObject{ { Tags::treeSpecies, reply.object().value(Tags::results).toArray() } });
    };
    sendRequest(QSharedPointer<AdditionalDataRequest>::create(AdditionalDataRequest::DataType::TreeSpecies),
                errorHandler, replyHandler);
}

void CharcoalSessionManager::getVillages()
{
    const auto errorHandler = [this](const QString &, const QNetworkReply::NetworkError &code) {
        emit additionalDataLoadError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &reply) {
        emit additionalDataLoaded(QJsonObject{ { Tags::villages, reply.object().value(Tags::results).toArray() } });
    };
    sendRequest(QSharedPointer<AdditionalDataRequest>::create(AdditionalDataRequest::DataType::Villages),
                errorHandler, replyHandler);
}
