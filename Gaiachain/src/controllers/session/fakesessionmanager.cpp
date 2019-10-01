#include "fakesessionmanager.h"

#include <QQmlApplicationEngine>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "../../common/tags.h"

FakeSessionManager::FakeSessionManager(QObject *parent)
    : AbstractSessionManager(parent)
{
    m_populator.populateFakeData(sc_initialShipmentsCount, QDateTime::currentDateTime().addDays(-sc_firstShipmentShift));
}

void FakeSessionManager::login(const QString &email, const QString &password)
{
    bool error = (qrand() % 100 == 1);

    updateConnectionStateBeforeRequest();
    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onLoginError() : onLogin(email, password); });
}

void FakeSessionManager::getEntity()
{
    emit beforeGetEntity();

    bool error = (qrand() % 100 == 1);

    updateConnectionStateBeforeRequest();
    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onEntityError() : onEntityAll(); });
}

void FakeSessionManager::getEntity(const QString &id)
{
    bool error = (qrand() % 100 == 1);

    updateConnectionStateBeforeRequest();
    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onEntityError() : onEntitySingle(id); });
}

void FakeSessionManager::putEntity(const QString &id, const Enums::SupplyChainAction &action, const Enums::ActionProgress &actionProgress)
{
    bool error = (qrand() % 100 == 1);

    updateConnectionStateBeforeRequest();
    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onEntitySaveError(id) : onEntitySaved(id, action, actionProgress); });
}

int FakeSessionManager::randomWaitTime()
{
    return qrand() % 1000;
}

void FakeSessionManager::onLogin(const QString &email, const QString &password)
{
    auto isSuccess = m_populator.checkLogin(email, password);
    auto error = isSuccess ? QNetworkReply::NoError : QNetworkReply::AuthenticationRequiredError;

    updateConnectionStateAfterRequest(error);
    if (isSuccess) {
        emit loginFinished(QJsonDocument::fromVariant(m_populator.generateUserData(email)) );
    } else {
        emit loginError(error);
    }
}

void FakeSessionManager::onLoginError()
{
    const auto error = QNetworkReply::HostNotFoundError;

    updateConnectionStateAfterRequest(error);
    emit loginError(error);
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

    auto shipmentsHistoryArray = QJsonArray{};
    auto shipmentsHistory = m_populator.getShipmentsHistory();
    for (const auto &shipmentId : shipmentsHistory.keys()) {
        auto shipmentHistory = QVariantMap({ { Tags::id, shipmentId },
                                             { Tags::history, shipmentsHistory.value(shipmentId) }});
        shipmentsHistoryArray.append(QJsonObject::fromVariantMap(shipmentHistory));
    }

    emit entitiesLoaded(shipmentsHistoryArray);
}

void FakeSessionManager::onEntitySingle(const QString &shipmentId)
{
    updateConnectionStateAfterRequest(QNetworkReply::NoError);

    auto shipmentsHistory = m_populator.getShipmentsHistory();
    if (!shipmentsHistory.contains(shipmentId)) {
        emit entityLoadError(-1);
        return;
    }

    auto shipmentHistory = QVariantMap({ { Tags::id, shipmentId },
                                         { Tags::history, shipmentsHistory.value(shipmentId) }});
    emit entityLoaded(QJsonObject::fromVariantMap(shipmentHistory));
}

void FakeSessionManager::onEntitySaveError(const QString &shipmentId)
{
    updateConnectionStateAfterRequest(QNetworkReply::HostNotFoundError);
    emit entitySaveResult(shipmentId, false);
}

void FakeSessionManager::onEntitySaved(const QString &shipmentId, const Enums::SupplyChainAction &action, const Enums::ActionProgress &actionProgress)
{
    updateConnectionStateAfterRequest(QNetworkReply::NoError);

    auto result = m_populator.canAddAction(shipmentId, action, actionProgress);
    emit entitySaveResult(shipmentId, result);
}
