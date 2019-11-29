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

    QTimer::singleShot(randomWaitTime(), this, [=]() { isError(error) ? onPingError(error) : onPingSuccess(); });
}

void FakeServer::login(const QString &login, const QString &password)
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() { isError(error) ? onLoginError(login, error) : onLogin(login, password); });
}

void FakeServer::getAdditionalData()
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() { isError(error) ? onAdditionalDataError(error) : onAdditionalData(); });
}

void FakeServer::getRelations(const QString &packageId)
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() { isError(error) ? onRelationsError(error) : onRelations({packageId, }); });
}

void FakeServer::getRelations(const QStringList &packageIds)
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() { isError(error) ? onRelationsError(error) : onRelations(packageIds); });
}

void FakeServer::addRelation(const QString &packageId, const QStringList &relatedIds)
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() { isError(error) ? onRelationSaveError(packageId, error) : onRelationSaved(packageId, relatedIds); });
}

void FakeServer::getEntitiesInfo(int count, const QDateTime &from, const QString &keyword)
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() { isError(error) ? onEntityError(error) : onEntityInfo(count, from, keyword); });
}

void FakeServer::getEntitiesInfo(const QDateTime &from, const QDateTime &to, const QString &keyword)
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() { isError(error) ? onEntityError(error) : onEntityInfo(from, to, keyword); });
}

void FakeServer::getEntities(const QStringList &packageIds)
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() { isError(error) ? onEntityError(error) : onEntity(packageIds); });
}

void FakeServer::getEntity(const QString &packageId)
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() { isError(error) ? onEntityError(error) : onEntity({ packageId, }); });
}

void FakeServer::getEntityId(const QByteArray &codeData)
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() { isError(error) ? onEntityError(error) : onEntityId(codeData); });
}

void FakeServer::putEntityAction(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties, const QByteArray &codeData)
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() { isError(error) ? onEntitySaveError(packageId, action, error) : onEntitySaved(packageId, action, timestamp, properties, codeData); });
}

void FakeServer::putEntityAction(const QByteArray &codeData, const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties)
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() { isError(error) ? onEntitySaveError({}, action, error) : onEntitySaved(QString{}, action, timestamp, properties, codeData); });
}

void FakeServer::postNewEntity(const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties, const QByteArray &codeData)
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() { isError(error) ? onEntitySaveError({}, action, error) : onEntitySaved(codeData, action, timestamp, properties); });
}

void FakeServer::getCreatedHarvestIds()
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() { isError(error) ? onCreatedHarvestIdsError(error) : onCreatedHarvestIds(); });
}

void FakeServer::getUnusedLotIds()
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() { isError(error) ? onUnusedLotIdsError(error) : onUnusedLotIds(); });
}

void FakeServer::postUnusedLotId()
{
    const auto error = randomError();

    QTimer::singleShot(randomWaitTime(), this, [=]() { isError(error) ? onUnusedLotIdCreationError(error) : onUnusedLotIdCreated(); });
}

bool FakeServer::isError(const QNetworkReply::NetworkError &error) const
{
    return (error != QNetworkReply::NoError);
}

QNetworkReply::NetworkError FakeServer::randomError() const
{
    auto error = !FakeServerState::instance().up() || (qrand() % 128 == 1);
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
    m_currentCooperativeId = QString{};

    emit loginError(login, error);
}

void FakeServer::onLogin(const QString &login, const QString &password)
{
    emit connectionState(QNetworkReply::NoError);

    if (m_populator.checkLogin(login, password)) {
        auto userData = m_populator.generateUserData(login);
        m_currentUserType = m_populator.userType(login);
        m_currentCooperativeId = userData.value(Tags::cooperativeId).toString();

        emit loginFinished(login, QJsonObject::fromVariantMap(userData));
    } else {
        m_currentUserType = Enums::UserType::Annonymous;
        m_currentCooperativeId = QString{};

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
    additionalData.insert(Tags::buyers, m_populator.getBuyers());
    additionalData.insert(Tags::transporters, m_populator.getTransporters());
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
                                               { Tags::packageId, data.first },
                                               { Tags::packageIds, data.second }
                                           });
    });
    emit relationsLoaded(relationsArray);
}

void FakeServer::onRelationSaveError(const QString &packageId, const QNetworkReply::NetworkError &error)
{
    emit connectionState(error);
    emit relationsSaveError(packageId, error);
}

void FakeServer::onRelationSaved(const QString &packageId, const QStringList &relatedIds)
{
    emit connectionState(QNetworkReply::NoError);

    m_populator.addPackageRelation(packageId, relatedIds);
    emit relationsSaved(packageId);
}

void FakeServer::onEntityError(const QNetworkReply::NetworkError &error)
{
    emit connectionState(error);
    emit entitiesLoadError(error);
}

void FakeServer::onEntityInfo(int count, const QDateTime &from, const QString &keyword)
{
    emit connectionState(QNetworkReply::NoError);

    emit entitiesInfoLoaded(QJsonArray::fromVariantList(m_populator.getEventsInfo(count, from, keyword)) );
}

void FakeServer::onEntityInfo(const QDateTime &from, const QDateTime &to, const QString &keyword)
{
    emit connectionState(QNetworkReply::NoError);

    emit entitiesInfoLoaded(QJsonArray::fromVariantList(m_populator.getEventsInfo(from, to, keyword)) );
}

void FakeServer::onEntity(const QStringList &packagesId)
{
    emit connectionState(QNetworkReply::NoError);

    emit entitiesLoaded(QJsonArray::fromVariantList(m_populator.getEventHistory(packagesId)) );
}

void FakeServer::onEntityIdError(const QNetworkReply::NetworkError &error)
{
    emit connectionState(error);
    emit entityIdLoadError(error);
}

void FakeServer::onEntityId(const QByteArray &codeData)
{
    emit connectionState(QNetworkReply::NoError);

    auto id = m_populator.getEventId(codeData);
    if (id.isEmpty()) {
        emit entityIdLoadError(QNetworkReply::HostNotFoundError);
    } else {
        emit entityIdLoaded(id);
    }
}

void FakeServer::onEntitySaveError(const QString &packageId, const Enums::SupplyChainAction &action, const QNetworkReply::NetworkError &error)
{
    emit connectionState(error);
    emit entitySaveError(packageId, action, error);
}

void FakeServer::onEntitySaved(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties, const QByteArray &codeData)
{
    emit connectionState(QNetworkReply::NoError);

    auto result = m_populator.addAction(packageId, action, timestamp, properties, codeData, m_currentUserType, m_currentCooperativeId);
    if (!result) {
        emit entitySaveError(packageId, action, QNetworkReply::NetworkError::ContentOperationNotPermittedError);
    } else {
        emit entitySaved(packageId, action);
    }
}

void FakeServer::onEntitySaved(const QByteArray &codeData, const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties)
{
    emit connectionState(QNetworkReply::NoError);

    auto packageId = m_populator.addNewAction(action, timestamp, properties, codeData, m_currentUserType, m_currentCooperativeId);
    if (packageId.isEmpty()) {
        emit entitySaveError({}, action, QNetworkReply::NetworkError::ContentOperationNotPermittedError);
    } else {
        emit entitySaved(packageId, action);
    }
}

void FakeServer::onCreatedHarvestIdsError(const QNetworkReply::NetworkError &error)
{
    emit connectionState(error);
    emit createdHarvestIdsLoadError(error);
}

void FakeServer::onCreatedHarvestIds()
{
    emit connectionState(QNetworkReply::NoError);

    emit createdHarvestIdsLoaded(QJsonArray::fromVariantList(m_populator.createdHarvestIds(m_currentCooperativeId)) );
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
