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

void FakeSessionManager::getRelations(const QString &id)
{
    bool error = (qrand() % 100 == 1);

    updateConnectionStateBeforeRequest();
    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onRelationsError() : onRelationsSingle(id); });
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

void FakeSessionManager::putEntity(const QString &id, const Enums::SupplyChainAction &action, const QVariantMap &properties)
{
    bool error = (qrand() % 100 == 1);

    updateConnectionStateBeforeRequest();
    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onEntitySaveError(id) : onEntitySaved(id, action, properties); });
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
    emit loginError(error);

    m_currentUserType = Enums::UserType::Annonymous;
}

void FakeSessionManager::onLogin(const QString &email, const QString &password)
{
    auto isSuccess = m_populator.checkLogin(email, password);
    auto error = isSuccess ? QNetworkReply::NoError : QNetworkReply::AuthenticationRequiredError;

    updateConnectionStateAfterRequest(error);
    if (isSuccess) {
        emit loginFinished(QJsonDocument::fromVariant(m_populator.generateUserData(email)) );
        m_currentUserType = m_populator.userType(email);
    } else {
        emit loginError(error);
        m_currentUserType = Enums::UserType::Annonymous;
    }
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

void FakeSessionManager::onEntitySaved(const QString &packageId, const Enums::SupplyChainAction &action, const QVariantMap &)
{
    updateConnectionStateAfterRequest(QNetworkReply::NoError);

    auto result = m_populator.canAddAction(packageId, action, m_currentUserType);
    emit entitySaveResult(packageId, result);
}
