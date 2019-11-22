#include "fakesessionmanager.h"

FakeSessionManager::FakeSessionManager(QObject *parent)
    : AbstractSessionManager(parent)
{
    connect(&m_fakeServer, &FakeServer::connectionState,
            this, [this](const int &error) { updateConnectionStateAfterRequest(static_cast<QNetworkReply::NetworkError>(error)); });

    connect(&m_fakeServer, &FakeServer::pingError, this, &FakeSessionManager::pingError);
    connect(&m_fakeServer, &FakeServer::pingSuccess, this, &FakeSessionManager::pingSuccess);

    connect(&m_fakeServer, &FakeServer::loginError, this, &FakeSessionManager::loginError);
    connect(&m_fakeServer, &FakeServer::loginFinished, this, &FakeSessionManager::loginFinished);

    connect(&m_fakeServer, &FakeServer::additionalDataLoadError, this, &FakeSessionManager::additionalDataLoadError);
    connect(&m_fakeServer, &FakeServer::additionalDataLoaded, this, &FakeSessionManager::additionalDataLoaded);

    connect(&m_fakeServer, &FakeServer::relationsLoadError, this, &FakeSessionManager::relationsLoadError);
    connect(&m_fakeServer, &FakeServer::relationsLoaded, this, &FakeSessionManager::relationsLoaded);
    connect(&m_fakeServer, &FakeServer::relationsSaveError, this, &FakeSessionManager::relationsSaveError);
    connect(&m_fakeServer, &FakeServer::relationsSaved, this, &FakeSessionManager::relationsSaved);

    connect(&m_fakeServer, &FakeServer::entitiesLoadError, this, &FakeSessionManager::entitiesLoadError);
    connect(&m_fakeServer, &FakeServer::entitiesLoaded, this, &FakeSessionManager::entitiesLoaded);
    connect(&m_fakeServer, &FakeServer::entitiesInfoLoaded, this, &FakeSessionManager::entitiesInfoLoaded);
    connect(&m_fakeServer, &FakeServer::entityIdLoadError, this, &FakeSessionManager::entityIdLoadError);
    connect(&m_fakeServer, &FakeServer::entityIdLoaded, this, &FakeSessionManager::entityIdLoaded);
    connect(&m_fakeServer, &FakeServer::entitySaveError, this, &FakeSessionManager::entitySaveError);
    connect(&m_fakeServer, &FakeServer::entitySaved, this, &FakeSessionManager::entitySaved);

    connect(&m_fakeServer, &FakeServer::createdHarvestIdsLoadError, this, &FakeSessionManager::createdHarvestIdsLoadError);
    connect(&m_fakeServer, &FakeServer::createdHarvestIdsLoaded, this, &FakeSessionManager::createdHarvestIdsLoaded);

    connect(&m_fakeServer, &FakeServer::unusedLotIdsLoadError, this, &FakeSessionManager::unusedLotIdsLoadError);
    connect(&m_fakeServer, &FakeServer::unusedLotIdsLoaded, this, &FakeSessionManager::unusedLotIdsLoaded);
    connect(&m_fakeServer, &FakeServer::unusedLotIdCreateError, this, &FakeSessionManager::unusedLotIdCreateError);
    connect(&m_fakeServer, &FakeServer::unusedLotIdCreated, this, &FakeSessionManager::unusedLotIdCreated);
}

void FakeSessionManager::ping()
{
    updateConnectionStateBeforeRequest();
    m_fakeServer.ping();
}

void FakeSessionManager::login(const QString &email, const QString &password)
{
    updateConnectionStateBeforeRequest();
    m_fakeServer.login(email, password);
}

void FakeSessionManager::getAdditionalData()
{
    updateConnectionStateBeforeRequest();
    m_fakeServer.getAdditionalData();
}

void FakeSessionManager::getRelations(const QString &packageId)
{
    updateConnectionStateBeforeRequest();
    m_fakeServer.getRelations(packageId);
}

void FakeSessionManager::getRelations(const QStringList &packageIds)
{
    updateConnectionStateBeforeRequest();
    m_fakeServer.getRelations(packageIds);
}

void FakeSessionManager::addRelation(const QString &packageId, const QStringList &relatedIds)
{
    updateConnectionStateBeforeRequest();
    m_fakeServer.addRelation(packageId, relatedIds);
}

void FakeSessionManager::getEntitiesInfo(int count, const QDateTime &from, const QString &keyword)
{
    updateConnectionStateBeforeRequest();
    m_fakeServer.getEntitiesInfo(count, from.isNull() ? QDateTime::currentDateTime() : from, keyword);
}

void FakeSessionManager::getEntitiesInfo(const QDateTime &from, const QDateTime &to, const QString &keyword)
{
    updateConnectionStateBeforeRequest();
    m_fakeServer.getEntitiesInfo(from, to, keyword);
}

void FakeSessionManager::getEntities(const QStringList &packageIds)
{
    updateConnectionStateBeforeRequest();
    m_fakeServer.getEntities(packageIds);
}

void FakeSessionManager::getEntity(const QString &packageId)
{
    updateConnectionStateBeforeRequest();
    m_fakeServer.getEntity(packageId);
}

void FakeSessionManager::getEntityId(const QByteArray &codeData)
{
    updateConnectionStateBeforeRequest();
    m_fakeServer.getEntityId(codeData);
}

void FakeSessionManager::putEntityAction(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties, const QByteArray &codeData)
{
    updateConnectionStateBeforeRequest();
    m_fakeServer.putEntityAction(packageId, action, timestamp, properties, codeData);
}

void FakeSessionManager::putEntityAction(const QByteArray &codeData, const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties)
{
    updateConnectionStateBeforeRequest();
    m_fakeServer.putEntityAction(codeData, action, timestamp, properties);
}

void FakeSessionManager::postNewEntity(const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties, const QByteArray &codeData)
{
    updateConnectionStateBeforeRequest();
    m_fakeServer.postNewEntity(action, timestamp, properties, codeData);
}

void FakeSessionManager::getCreatedHarvestIds()
{
    updateConnectionStateBeforeRequest();
    m_fakeServer.getCreatedHarvestIds();
}

void FakeSessionManager::getUnusedLotIds()
{
    updateConnectionStateBeforeRequest();
    m_fakeServer.getUnusedLotIds();
}

void FakeSessionManager::postUnusedLotId()
{
    updateConnectionStateBeforeRequest();
    m_fakeServer.postUnusedLotId();
}
