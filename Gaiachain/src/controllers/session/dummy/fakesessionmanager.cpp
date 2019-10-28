#include "fakesessionmanager.h"

#include <QQmlApplicationEngine>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "../../../common/tags.h"
#include "../../../helpers/utility.h"

FakeSessionManager::FakeSessionManager(QObject *parent)
    : AbstractSessionManager(parent)
{
    m_populator.populateFakeData(QDate::currentDate().addDays(-sc_firstHarvestShift));
}

void FakeSessionManager::login(const QString &email, const QString &password)
{
    bool error = (qrand() % 100 == 1);

    updateConnectionStateBeforeRequest();
    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onLoginError() : onLogin(email, password); });
}

void FakeSessionManager::getAdditionalData()
{
    bool error = (qrand() % 100 == 1);

    updateConnectionStateBeforeRequest();
    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onAdditionalDataError() : onAdditionalData(); });
}

void FakeSessionManager::getRelations(const QString &id)
{
    bool error = (qrand() % 100 == 1);

    updateConnectionStateBeforeRequest();
    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onRelationsError() : onRelationsSingle(id); });
}

void FakeSessionManager::addRelation(const QString &id, const QStringList &ids)
{
    bool error = (qrand() % 100 == 1);

    updateConnectionStateBeforeRequest();
    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onRelationSaveError(id) : onRelationSaved(id, ids); });
}

void FakeSessionManager::getEntitiesInfo(int count, const QDateTime &from)
{
    Q_UNUSED(count)
    Q_UNUSED(from)
    Q_ASSERT_X(false, "Not implemented", "Not implemented for fake data");
}

void FakeSessionManager::getEntitiesInfo(const QDateTime &to, const QDateTime &from)
{
    Q_UNUSED(to)
    Q_UNUSED(from)
    Q_ASSERT_X(false, "Not implemented", "Not implemented for fake data");
}

void FakeSessionManager::getEntities(const QStringList &ids)
{
    bool error = (qrand() % 100 == 1);

    updateConnectionStateBeforeRequest();
    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onEntityError() : onEntityMultiple(ids); });
}

void FakeSessionManager::getEntity(const QString &id)
{
    bool error = (qrand() % 100 == 1);

    updateConnectionStateBeforeRequest();
    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onEntityError() : onEntitySingle(id); });
}

void FakeSessionManager::getEntitId(const QByteArray &codeData)
{
    Q_UNUSED(codeData)
    Q_ASSERT_X(false, "Not implemented", "Not implemented for fake data");
}

void FakeSessionManager::putEntityAction(const QString &id, const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties, const QByteArray &codeData)
{
    bool error = (qrand() % 100 == 1);

    updateConnectionStateBeforeRequest();
    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onEntitySaveError(id) : onEntitySaved(id, action, timestamp, properties, codeData); });
}

void FakeSessionManager::putEntity(const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties, const QByteArray &codeData)
{
    bool error = (qrand() % 100 == 1);

    updateConnectionStateBeforeRequest();
    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onEntitySaveError({}) : onEntitySaved(action, timestamp, properties, codeData); });
}

void FakeSessionManager::getUnusedLotIds()
{
    bool error = (qrand() % 100 == 1);

    updateConnectionStateBeforeRequest();
    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onUnusedLotIdsError() : onUnusedLotIds(); });
}

void FakeSessionManager::createUnusedLotId()
{
    bool error = (qrand() % 100 == 1);

    updateConnectionStateBeforeRequest();
    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onUnusedLotIdCreationError() : onUnusedLotIdCreated(); });
}

void FakeSessionManager::getAllRelations()
{
    bool error = (qrand() % 100 == 1);

    updateConnectionStateBeforeRequest();
    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onRelationsError() : onRelationsAll(); });
}

void FakeSessionManager::getAllEntities()
{
    bool error = (qrand() % 100 == 1);

    updateConnectionStateBeforeRequest();
    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onEntityError() : onEntityAll(); });
}

int FakeSessionManager::randomWaitTime()
{
    return qrand() % 1000;
}

void FakeSessionManager::onLoginError()
{
    const auto error = QNetworkReply::HostNotFoundError;

    updateConnectionStateAfterRequest(error);

    m_currentUserType = Enums::UserType::Annonymous;
    m_currentCooperativeId = QString{};

    emit loginError(error);
}

void FakeSessionManager::onLogin(const QString &email, const QString &password)
{
    auto isSuccess = m_populator.checkLogin(email, password);
    auto error = isSuccess ? QNetworkReply::NoError : QNetworkReply::AuthenticationRequiredError;

    updateConnectionStateAfterRequest(error);
    if (isSuccess) {
        auto userData = m_populator.generateUserData(email);
        m_currentUserType = m_populator.userType(email);
        m_currentCooperativeId = userData.value(Tags::cooperativeId).toString();

        emit loginFinished(QJsonDocument::fromVariant(userData));
    } else {
        m_currentUserType = Enums::UserType::Annonymous;
        m_currentCooperativeId = QString{};

        emit loginError(error);
    }
}

void FakeSessionManager::onAdditionalDataError()
{
    const auto error = QNetworkReply::HostNotFoundError;

    updateConnectionStateAfterRequest(error);
    emit additionalDataLoadError(error);
}

void FakeSessionManager::onAdditionalData()
{
    updateConnectionStateAfterRequest(QNetworkReply::NoError);

    auto additionalData = QVariantMap{};
    additionalData.insert(Tags::producers, m_populator.getProducers());
    additionalData.insert(Tags::buyers, m_populator.getBuyers());
    additionalData.insert(Tags::transporters, m_populator.getTransporters());
    additionalData.insert(Tags::destinations, m_populator.getDestinations());

    emit additionalDataLoaded(QJsonObject::fromVariantMap(additionalData));
}

void FakeSessionManager::onRelationsError()
{
    const auto error = QNetworkReply::HostNotFoundError;

    updateConnectionStateAfterRequest(error);
    emit relationsLoadError(error);
}

void FakeSessionManager::onRelationsAll()
{
    updateConnectionStateAfterRequest(QNetworkReply::NoError);

    emit packagesRelationsLoaded(QJsonObject::fromVariantMap(m_populator.getPackagesRelations()) );
}

void FakeSessionManager::onRelationsSingle(const QString &packageId)
{
    updateConnectionStateAfterRequest(QNetworkReply::NoError);

    emit packageRelationsLoaded(QJsonArray::fromVariantList(m_populator.getPackageRelations(packageId)) );
}

void FakeSessionManager::onRelationSaveError(const QString &packageId)
{
    updateConnectionStateAfterRequest(QNetworkReply::HostNotFoundError);
    emit packageRelationsSaveResult(packageId, false);
}

void FakeSessionManager::onRelationSaved(const QString &packageId, const QStringList &relatedIds)
{
    updateConnectionStateAfterRequest(QNetworkReply::NoError);

    m_populator.addPackageRelation(packageId, relatedIds);
    emit packageRelationsSaveResult(packageId, true);
}

void FakeSessionManager::onEntityError()
{
    const auto error = QNetworkReply::HostNotFoundError;

    updateConnectionStateAfterRequest(error);
    emit entityLoadError(error);
}

void FakeSessionManager::onEntityAll()
{
    updateConnectionStateAfterRequest(QNetworkReply::NoError);

    emit entitiesLoaded(QJsonArray::fromVariantList(m_populator.getEventsHistory()) );
}

void FakeSessionManager::onEntityMultiple(const QStringList &packagesId)
{
    updateConnectionStateAfterRequest(QNetworkReply::NoError);

    emit entitiesLoaded(QJsonArray::fromVariantList(m_populator.getEventHistory(packagesId)) );
}

void FakeSessionManager::onEntitySingle(const QString &packageId)
{
    updateConnectionStateAfterRequest(QNetworkReply::NoError);

    emit entitiesLoaded(QJsonArray::fromVariantList(m_populator.getEventHistory(packageId)) );
}

void FakeSessionManager::onEntitySaveError(const QString &packageId)
{
    updateConnectionStateAfterRequest(QNetworkReply::HostNotFoundError);
    emit entitySaveResult(packageId, false);
}

void FakeSessionManager::onEntitySaved(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties, const QByteArray &codeData)
{
    updateConnectionStateAfterRequest(QNetworkReply::NoError);

    auto result = m_populator.addAction(packageId, action, timestamp, properties, codeData, m_currentUserType, m_currentCooperativeId);
    emit entitySaveResult(packageId, result);
}

void FakeSessionManager::onEntitySaved(const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties, const QByteArray &codeData)
{
    updateConnectionStateAfterRequest(QNetworkReply::NoError);

    auto packageId = m_populator.addNewAction(action, timestamp, properties, codeData, m_currentUserType, m_currentCooperativeId);
    emit entitySaveResult(packageId, !packageId.isEmpty());
}

void FakeSessionManager::onUnusedLotIdsError()
{
    const auto error = QNetworkReply::HostNotFoundError;

    updateConnectionStateAfterRequest(error);
    emit unusedLotIdsLoadError(error);
}

void FakeSessionManager::onUnusedLotIds()
{
    updateConnectionStateAfterRequest(QNetworkReply::NoError);

    emit unusedLotIdsLoaded(QJsonArray::fromVariantList(m_populator.unusedLotIds(m_currentCooperativeId)) );
}

void FakeSessionManager::onUnusedLotIdCreationError()
{
    updateConnectionStateAfterRequest(QNetworkReply::HostNotFoundError);
    emit createUnusedLotIdResult({}, false);
}

void FakeSessionManager::onUnusedLotIdCreated()
{
    updateConnectionStateAfterRequest(QNetworkReply::NoError);

    emit createUnusedLotIdResult(m_populator.createUnusedLotId(m_currentCooperativeId), true);
}
