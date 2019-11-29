#include "fakesessionmanager.h"

#include <QJsonObject>

FakeSessionManager::FakeSessionManager(QObject *parent)
    : AbstractSessionManager(parent)
{
    connect(&FakeServer::instance(), &FakeServer::connectionState,
            this, [this](const int &error) { updateConnectionStateAfterRequest(static_cast<QNetworkReply::NetworkError>(error)); });

    connect(&FakeServer::instance(), &FakeServer::pingError, this, &FakeSessionManager::pingError);
    connect(&FakeServer::instance(), &FakeServer::pingSuccess, this, &FakeSessionManager::pingSuccess);

    connect(&FakeServer::instance(), &FakeServer::loginError, this, &FakeSessionManager::loginError);
    connect(&FakeServer::instance(), &FakeServer::loginFinished, this, &FakeSessionManager::loginFinished);

    connect(&FakeServer::instance(), &FakeServer::additionalDataLoadError, this, &FakeSessionManager::additionalDataLoadError);
    connect(&FakeServer::instance(), &FakeServer::additionalDataLoaded, this, &FakeSessionManager::additionalDataLoaded);

    connect(&FakeServer::instance(), &FakeServer::relationsLoadError, this, &FakeSessionManager::relationsLoadError);
    connect(&FakeServer::instance(), &FakeServer::relationsLoaded, this, &FakeSessionManager::relationsLoaded);
    connect(&FakeServer::instance(), &FakeServer::relationsSaveError, this, &FakeSessionManager::relationsSaveError);
    connect(&FakeServer::instance(), &FakeServer::relationsSaved, this, &FakeSessionManager::relationsSaved);

    connect(&FakeServer::instance(), &FakeServer::entitiesLoadError, this, &FakeSessionManager::entitiesLoadError);
    connect(&FakeServer::instance(), &FakeServer::entitiesLoaded, this, &FakeSessionManager::entitiesLoaded);
    connect(&FakeServer::instance(), &FakeServer::entitiesInfoLoaded, this, &FakeSessionManager::entitiesInfoLoaded);
    connect(&FakeServer::instance(), &FakeServer::entityIdLoadError, this, &FakeSessionManager::entityIdLoadError);
    connect(&FakeServer::instance(), &FakeServer::entityIdLoaded, this, &FakeSessionManager::entityIdLoaded);
    connect(&FakeServer::instance(), &FakeServer::entitySaveError, this, &FakeSessionManager::entitySaveError);
    connect(&FakeServer::instance(), &FakeServer::entitySaved, this, &FakeSessionManager::entitySaved);

    connect(&FakeServer::instance(), &FakeServer::createdHarvestIdsLoadError, this, &FakeSessionManager::createdHarvestIdsLoadError);
    connect(&FakeServer::instance(), &FakeServer::createdHarvestIdsLoaded, this, &FakeSessionManager::createdHarvestIdsLoaded);

    connect(&FakeServer::instance(), &FakeServer::unusedLotIdsLoadError, this, &FakeSessionManager::unusedLotIdsLoadError);
    connect(&FakeServer::instance(), &FakeServer::unusedLotIdsLoaded, this, &FakeSessionManager::unusedLotIdsLoaded);
    connect(&FakeServer::instance(), &FakeServer::unusedLotIdCreateError, this, &FakeSessionManager::unusedLotIdCreateError);
    connect(&FakeServer::instance(), &FakeServer::unusedLotIdCreated, this, &FakeSessionManager::unusedLotIdCreated);
}

void FakeSessionManager::ping()
{
    updateConnectionStateBeforeRequest();
    FakeServer::instance().ping();
}

void FakeSessionManager::login(const QString &email, const QString &password)
{
    emit loginAttempt(email, password);

    updateConnectionStateBeforeRequest();
    FakeServer::instance().login(email, password);
}

void FakeSessionManager::getAdditionalData()
{
    updateConnectionStateBeforeRequest();
    FakeServer::instance().getAdditionalData();
}

void FakeSessionManager::getRelations(const QString &packageId)
{
    updateConnectionStateBeforeRequest();
    FakeServer::instance().getRelations(packageId);
}

void FakeSessionManager::getRelations(const QStringList &packageIds)
{
    updateConnectionStateBeforeRequest();
    FakeServer::instance().getRelations(packageIds);
}

void FakeSessionManager::addRelation(const QString &packageId, const QStringList &relatedIds)
{
    updateConnectionStateBeforeRequest();
    FakeServer::instance().addRelation(packageId, relatedIds);
}

void FakeSessionManager::getEntitiesInfo(int count, const QDateTime &from, const QString &keyword)
{
    updateConnectionStateBeforeRequest();
    FakeServer::instance().getEntitiesInfo(count, from.isNull() ? QDateTime::currentDateTime() : from, keyword);
}

void FakeSessionManager::getEntitiesInfo(const QDateTime &from, const QDateTime &to, const QString &keyword)
{
    updateConnectionStateBeforeRequest();
    FakeServer::instance().getEntitiesInfo(from, to, keyword);
}

void FakeSessionManager::getEntities(const QStringList &packageIds)
{
    updateConnectionStateBeforeRequest();
    FakeServer::instance().getEntities(packageIds);
}

void FakeSessionManager::getEntity(const QString &packageId)
{
    updateConnectionStateBeforeRequest();
    FakeServer::instance().getEntity(packageId);
}

void FakeSessionManager::getEntityId(const QByteArray &codeData)
{
    updateConnectionStateBeforeRequest();
    FakeServer::instance().getEntityId(codeData);
}

void FakeSessionManager::putEntityAction(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties, const QByteArray &codeData)
{
    updateConnectionStateBeforeRequest();
    FakeServer::instance().putEntityAction(packageId, action, timestamp, properties, codeData);
}

void FakeSessionManager::putEntityAction(const QByteArray &codeData, const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties)
{
    updateConnectionStateBeforeRequest();
    FakeServer::instance().putEntityAction(codeData, action, timestamp, properties);
}

void FakeSessionManager::postNewEntity(const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties, const QByteArray &codeData)
{
    updateConnectionStateBeforeRequest();
    FakeServer::instance().postNewEntity(action, timestamp, properties, codeData);
}

void FakeSessionManager::getCreatedHarvestIds()
{
    updateConnectionStateBeforeRequest();
    FakeServer::instance().getCreatedHarvestIds();
}

void FakeSessionManager::getUnusedLotIds()
{
    updateConnectionStateBeforeRequest();
    FakeServer::instance().getUnusedLotIds();
}

void FakeSessionManager::postUnusedLotId()
{
    updateConnectionStateBeforeRequest();
    FakeServer::instance().postUnusedLotId();
}
