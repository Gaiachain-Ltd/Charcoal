#include "fakeserver.h"

#include <QQmlApplicationEngine>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "fakeserverstate.h"
#include "../../../common/tags.h"
#include "../../../helpers/utility.h"

FakeServer::FakeServer(QObject *parent)
    : QObject(parent)
{
    m_thread.start();
    moveToThread(&m_thread);

    m_populator.populateFakeData(QDate::currentDate().addDays(-sc_firstHarvestShift));
}

FakeServer::~FakeServer()
{
    m_thread.quit();
    if (!m_thread.wait()) {
        m_thread.terminate();
    }
}

FakeServer &FakeServer::instance()
{
    static FakeServer server;
    return server;
}

void FakeServer::ping()
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() {
        isError(error) ? onPingError(error) : onPingSuccess();
        emit requestFinished();
    });
}

void FakeServer::login(const QString &login, const QString &password)
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() {
        isError(error) ? onLoginError(login, error) : onLogin(login, password);
        emit requestFinished();
    });
}

void FakeServer::getAdditionalData()
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() {
        isError(error) ? onAdditionalDataError(error) : onAdditionalData();
        emit requestFinished();
    });
}

void FakeServer::getProducers()
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() {
        isError(error) ? onAdditionalDataError(error) : onProducers();
        emit requestFinished();
    });
}

void FakeServer::getCompanies()
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() {
        isError(error) ? onAdditionalDataError(error) : onCompanies();
        emit requestFinished();
    });
}

void FakeServer::getDestinations()
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() {
        isError(error) ? onAdditionalDataError(error) : onDestinations();
        emit requestFinished();
    });
}

void FakeServer::getRelations(const QStringList &packageIds)
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() {
        isError(error) ? onRelationsError(error) : onRelations(packageIds);
        emit requestFinished();
    });
}

void FakeServer::getEntitiesInfo(int limit, const QDateTime &to, const QString &keyword)
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() {
        isError(error) ? onEntityError(error) : onEntityInfo(limit, to, keyword);
        emit requestFinished();
    });
}

void FakeServer::getEntitiesInfo(const QDateTime &from, const QDateTime &to, const QString &keyword)
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() {
        isError(error) ? onEntityError(error) : onEntityInfo(from, to, keyword);
        emit requestFinished();
    });
}

void FakeServer::getLastActionEntitiesInfo(const Enums::SupplyChainAction &lastAction)
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() {
        isError(error) ? onEntityError(error) : onLastActionEntitiesInfo(lastAction);
        emit requestFinished();
    });
}

void FakeServer::getEntities(const QStringList &packageIds)
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() {
        isError(error) ? onEntityError(error) : onEntity(packageIds);
        emit requestFinished();
    });
}

void FakeServer::postNewEntity(const QString &packageId, const Enums::SupplyChainAction &action,
                               const QDateTime &timestamp, const QVariantMap &properties)
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() {
        isError(error) ? onEntitySaveError({}, action, error) : onEntitySaved(packageId, action, timestamp, properties);
        emit requestFinished();
    });
}

void FakeServer::postNewEntity(const QString &packageId, const QByteArray &codeData, const Enums::SupplyChainAction &action,
                               const QDateTime &timestamp, const QVariantMap &properties)
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() {
        isError(error) ? onEntitySaveError({}, action, error) : onEntitySaved(packageId, codeData, action, timestamp, properties);
        emit requestFinished();
    });
}

void FakeServer::postNewEntity(const QByteArray &codeData, const Enums::SupplyChainAction &action,
                               const QDateTime &timestamp, const QVariantMap &properties)
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() {
        isError(error) ? onEntitySaveError({}, action, error) : onEntitySaved(codeData, action, timestamp, properties);
        emit requestFinished();
    });
}

void FakeServer::getUnusedLotIds()
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() {
        isError(error) ? onUnusedLotIdsError(error) : onUnusedLotIds();
        emit requestFinished();
    });
}

void FakeServer::postUnusedLotId()
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() {
        isError(error) ? onUnusedLotIdCreationError(error) : onUnusedLotIdCreated();
        emit requestFinished();
    });
}

bool FakeServer::isError(const QNetworkReply::NetworkError &error) const
{
    return (error != QNetworkReply::NoError);
}

QNetworkReply::NetworkError FakeServer::randomError() const
{
    auto error = !FakeServerState::instance().up() || (qrand() % 1024 == 1);
    return error ? QNetworkReply::HostNotFoundError : QNetworkReply::NoError;
}

int FakeServer::randomWaitTime() const
{
    return qrand() % 1000;
}

void FakeServer::onPingError(const QNetworkReply::NetworkError &error)
{
    emit connectionState(error);
    emit pingError(error);
}

void FakeServer::onPingSuccess()
{
    emit connectionState(QNetworkReply::NoError);
    emit pingSuccess();
}

void FakeServer::onLoginError(const QString &login, const QNetworkReply::NetworkError &error)
{
    emit connectionState(error);

    m_currentUserType = Enums::UserType::Annonymous;
    m_currentCooperativeId = -1;

    emit loginError(login, error);
}

void FakeServer::onLogin(const QString &login, const QString &password)
{
    emit connectionState(QNetworkReply::NoError);

    if (m_populator.checkLogin(login, password)) {
        auto userData = m_populator.generateUserData(login);
        m_currentUserType = m_populator.userType(login);
        m_currentCooperativeId = userData.value(Tags::company).toMap().value(Tags::id).toInt();

        emit loginFinished(login, QJsonObject::fromVariantMap(userData));
    } else {
        m_currentUserType = Enums::UserType::Annonymous;
        m_currentCooperativeId = -1;

        emit loginError(login, QNetworkReply::AuthenticationRequiredError);
    }
}

void FakeServer::onAdditionalDataError(const QNetworkReply::NetworkError &error)
{
    emit connectionState(error);
    emit additionalDataLoadError(error);
}

void FakeServer::onAdditionalData()
{
    emit connectionState(QNetworkReply::NoError);

    auto additionalData = QVariantMap{};
    additionalData.insert(Tags::producers, m_populator.getProducers());
    additionalData.insert(Tags::companies, m_populator.getCompanies());
    additionalData.insert(Tags::destinations, m_populator.getDestinations());

    emit additionalDataLoaded(QJsonObject::fromVariantMap(additionalData));
}

void FakeServer::onProducers()
{
    emit connectionState(QNetworkReply::NoError);

    auto additionalData = QVariantMap{};
    additionalData.insert(Tags::producers, m_populator.getProducers());

    emit additionalDataLoaded(QJsonObject::fromVariantMap(additionalData));
}

void FakeServer::onCompanies()
{
    emit connectionState(QNetworkReply::NoError);

    auto additionalData = QVariantMap{};
    additionalData.insert(Tags::companies, m_populator.getCompanies());

    emit additionalDataLoaded(QJsonObject::fromVariantMap(additionalData));
}

void FakeServer::onDestinations()
{
    emit connectionState(QNetworkReply::NoError);

    auto additionalData = QVariantMap{};
    additionalData.insert(Tags::destinations, m_populator.getDestinations());

    emit additionalDataLoaded(QJsonObject::fromVariantMap(additionalData));
}

void FakeServer::onRelationsError(const QNetworkReply::NetworkError &error)
{
    emit connectionState(error);
    emit relationsLoadError(error);
}

void FakeServer::onRelations(const QStringList &packagesIds)
{
    emit connectionState(QNetworkReply::NoError);

    auto relationsMap = m_populator.getRelations(packagesIds);
    auto relationsArray = QJsonArray{};
    std::transform(relationsMap.constKeyValueBegin(), relationsMap.constKeyValueEnd(),
                   std::back_inserter(relationsArray),
                   [](const std::pair<QString, QVariant> &data) -> QJsonValue {
        return QJsonObject::fromVariantMap({
                                               { Tags::pid, data.first },
                                               { Tags::pids, data.second }
                                           });
    });
    emit relationsLoaded(relationsArray);
}

void FakeServer::onEntityError(const QNetworkReply::NetworkError &error)
{
    emit connectionState(error);
    emit entitiesLoadError(error);
}

void FakeServer::onEntityInfo(int limit, const QDateTime &to, const QString &keyword)
{
    emit connectionState(QNetworkReply::NoError);

    emit entitiesInfoLoaded(QJsonArray::fromVariantList(m_populator.getEventsInfo(limit, to, keyword)) );
}

void FakeServer::onEntityInfo(const QDateTime &from, const QDateTime &to, const QString &keyword)
{
    emit connectionState(QNetworkReply::NoError);

    emit entitiesInfoLoaded(QJsonArray::fromVariantList(m_populator.getEventsInfo(from, to, keyword)) );
}

void FakeServer::onLastActionEntitiesInfo(const Enums::SupplyChainAction &lastAction)
{
    emit connectionState(QNetworkReply::NoError);

    emit entitiesInfoLoaded(QJsonArray::fromVariantList(m_populator.getLastActionEventsInfo(m_currentCooperativeId, lastAction)) );
}

void FakeServer::onEntity(const QStringList &packagesId)
{
    emit connectionState(QNetworkReply::NoError);

    emit entitiesLoaded(QJsonArray::fromVariantList(m_populator.getEventsHistory(packagesId)) );
}

void FakeServer::onEntitySaveError(const QString &packageId, const Enums::SupplyChainAction &action, const QNetworkReply::NetworkError &error)
{
    emit connectionState(error);
    emit entitySaveError(packageId, action, error);
}

void FakeServer::onEntitySaved(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties)
{
    emit connectionState(QNetworkReply::NoError);

    auto result = m_populator.addAction(packageId, action, timestamp, properties, m_currentUserType, m_currentCooperativeId);
    if (!result) {
        emit entitySaveError(packageId, action, QNetworkReply::NetworkError::ContentOperationNotPermittedError);
    } else {
        emit entitySaved(packageId, action);
    }
}

void FakeServer::onEntitySaved(const QString &packageId, const QByteArray &codeData, const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties)
{
    emit connectionState(QNetworkReply::NoError);

    auto result = m_populator.addAction(packageId, codeData, action, timestamp, properties, m_currentUserType, m_currentCooperativeId);
    if (!result) {
        emit entitySaveError({}, action, QNetworkReply::NetworkError::ContentOperationNotPermittedError);
    } else {
        emit entitySaved(packageId, action);
    }
}

void FakeServer::onEntitySaved(const QByteArray &codeData, const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties)
{
    emit connectionState(QNetworkReply::NoError);

    auto packageId = m_populator.addAction(codeData, action, timestamp, properties, m_currentUserType, m_currentCooperativeId);
    if (packageId.isEmpty()) {
        emit entitySaveError({}, action, QNetworkReply::NetworkError::ContentOperationNotPermittedError);
    } else {
        emit entitySaved(packageId, action);
    }
}

void FakeServer::onUnusedLotIdsError(const QNetworkReply::NetworkError &error)
{
    emit connectionState(error);
    emit unusedLotIdsLoadError(error);
}

void FakeServer::onUnusedLotIds()
{
    emit connectionState(QNetworkReply::NoError);

    emit unusedLotIdsLoaded(QJsonArray::fromVariantList(m_populator.unusedLotIds(m_currentCooperativeId)) );
}

void FakeServer::onUnusedLotIdCreationError(const QNetworkReply::NetworkError &error)
{
    emit connectionState(error);
    emit unusedLotIdCreateError(error);
}

void FakeServer::onUnusedLotIdCreated()
{
    emit connectionState(QNetworkReply::NoError);

    emit unusedLotIdCreated(m_populator.createUnusedLotId(m_currentCooperativeId));
}
