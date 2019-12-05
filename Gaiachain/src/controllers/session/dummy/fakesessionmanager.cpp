#include "fakesessionmanager.h"

#include <QJsonObject>

FakeSessionManager::FakeSessionManager(QObject *parent)
    : AbstractSessionManager(parent)
{
    connect(&FakeServer::instance(), &FakeServer::connectionState,
            this, [this](const int &error) { updateConnectionStateAfterRequest(static_cast<QNetworkReply::NetworkError>(error)); });
    connect(&FakeServer::instance(), &FakeServer::requestFinished,
            this, [this]() { emit processFinished(); });    // cannot connect directly because processFinished is protected

    connect(&FakeServer::instance(), &FakeServer::pingError, this, &FakeSessionManager::pingError);
    connect(&FakeServer::instance(), &FakeServer::pingSuccess, this, &FakeSessionManager::pingSuccess);

    connect(&FakeServer::instance(), &FakeServer::loginError, this, &FakeSessionManager::loginError);
    connect(&FakeServer::instance(), &FakeServer::loginFinished, this, &FakeSessionManager::loginFinished);

    connect(&FakeServer::instance(), &FakeServer::additionalDataLoadError, this, &FakeSessionManager::additionalDataLoadError);
    connect(&FakeServer::instance(), &FakeServer::additionalDataLoaded, this, &FakeSessionManager::additionalDataLoaded);

    connect(&FakeServer::instance(), &FakeServer::relationsLoadError, this, &FakeSessionManager::relationsLoadError);
    connect(&FakeServer::instance(), &FakeServer::relationsLoaded, this, &FakeSessionManager::relationsLoaded);

    connect(&FakeServer::instance(), &FakeServer::entitiesLoadError, this, &FakeSessionManager::entitiesLoadError);
    connect(&FakeServer::instance(), &FakeServer::entitiesLoaded, this, &FakeSessionManager::entitiesLoaded);
    connect(&FakeServer::instance(), &FakeServer::entitiesInfoLoaded, this, &FakeSessionManager::entitiesInfoLoaded);
    connect(&FakeServer::instance(), &FakeServer::entitySaveError, this, &FakeSessionManager::entitySaveError);
    connect(&FakeServer::instance(), &FakeServer::entitySaved, this, &FakeSessionManager::entitySaved);

    connect(&FakeServer::instance(), &FakeServer::unusedLotIdsLoadError, this, &FakeSessionManager::unusedLotIdsLoadError);
    connect(&FakeServer::instance(), &FakeServer::unusedLotIdsLoaded, this, &FakeSessionManager::unusedLotIdsLoaded);
    connect(&FakeServer::instance(), &FakeServer::unusedLotIdCreateError, this, &FakeSessionManager::unusedLotIdCreateError);
    connect(&FakeServer::instance(), &FakeServer::unusedLotIdCreated, this, &FakeSessionManager::unusedLotIdCreated);
}

void FakeSessionManager::ping()
{
    processStarted();
    updateConnectionStateBeforeRequest();
    FakeServer::instance().ping();
}

void FakeSessionManager::login(const QString &email, const QString &password)
{
    emit loginAttempt(email, password);

    processStarted();
    updateConnectionStateBeforeRequest();
    FakeServer::instance().login(email, password);
}

void FakeSessionManager::getAdditionalData()
{
    processStarted();
    updateConnectionStateBeforeRequest();
    FakeServer::instance().getAdditionalData();
}

void FakeSessionManager::getProducers()
{
    processStarted();
    updateConnectionStateBeforeRequest();
    FakeServer::instance().getProducers();
}

void FakeSessionManager::getCompanies()
{
    processStarted();
    updateConnectionStateBeforeRequest();
    FakeServer::instance().getCompanies();
}

void FakeSessionManager::getDestinations()
{
    processStarted();
    updateConnectionStateBeforeRequest();
    FakeServer::instance().getDestinations();
}

void FakeSessionManager::getRelations(const QStringList &packageIds)
{
    processStarted();
    updateConnectionStateBeforeRequest();
    FakeServer::instance().getRelations(packageIds);
}

void FakeSessionManager::getEntitiesInfo(int limit, const QDateTime &to, const QString &keyword)
{
    processStarted();
    updateConnectionStateBeforeRequest();
    FakeServer::instance().getEntitiesInfo(limit, to.isNull() ? QDateTime::currentDateTime() : to, keyword);
}

void FakeSessionManager::getEntitiesInfo(const QDateTime &from, const QDateTime &to, const QString &keyword)
{
    processStarted();
    updateConnectionStateBeforeRequest();
    FakeServer::instance().getEntitiesInfo(from, to, keyword);
}

void FakeSessionManager::getLastActionEntitiesInfo(const Enums::SupplyChainAction &lastAction)
{
    processStarted();
    updateConnectionStateBeforeRequest();
    FakeServer::instance().getLastActionEntitiesInfo(lastAction);
}

void FakeSessionManager::getEntities(const QStringList &packageIds)
{
    processStarted();
    updateConnectionStateBeforeRequest();
    FakeServer::instance().getEntities(packageIds);
}

void FakeSessionManager::postNewEntity(const QString &packageId, const Enums::SupplyChainAction &action,
                                       const QDateTime &timestamp, const QVariantMap &properties)
{
    processStarted();
    updateConnectionStateBeforeRequest();
    FakeServer::instance().postNewEntity(packageId, action, timestamp, properties);
}

void FakeSessionManager::postNewEntity(const QString &packageId, const QByteArray &codeData, const Enums::SupplyChainAction &action,
                                       const QDateTime &timestamp, const QVariantMap &properties)
{
    processStarted();
    updateConnectionStateBeforeRequest();
    FakeServer::instance().postNewEntity(packageId, codeData, action, timestamp, properties);
}

void FakeSessionManager::postNewEntity(const QByteArray &codeData, const Enums::SupplyChainAction &action,
                                       const QDateTime &timestamp, const QVariantMap &properties)
{
    processStarted();
    updateConnectionStateBeforeRequest();
    FakeServer::instance().postNewEntity(codeData, action, timestamp, properties);
}

void FakeSessionManager::getUnusedLotIds()
{
    processStarted();
    updateConnectionStateBeforeRequest();
    FakeServer::instance().getUnusedLotIds();
}

void FakeSessionManager::postUnusedLotId()
{
    processStarted();
    updateConnectionStateBeforeRequest();
    FakeServer::instance().postUnusedLotId();
}
