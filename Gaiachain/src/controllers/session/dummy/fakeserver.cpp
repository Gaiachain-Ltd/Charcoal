#include "fakeserver.h"

#include <QQmlApplicationEngine>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "../../../common/tags.h"
#include "../../../helpers/utility.h"

FakeServer::FakeServer(QObject *parent)
    : QObject(parent)
{
    m_thread.start();
    moveToThread(&m_thread);

    m_populator.populateFakeData(QDate::currentDate().addDays(-sc_firstHarvestShift));
}

void FakeServer::ping()
{
    bool error = (qrand() % 100 == 1);

    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onPingError() : onPingSuccess(); });
}

void FakeServer::login(const QString &email, const QString &password)
{
    bool error = (qrand() % 100 == 1);

    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onLoginError() : onLogin(email, password); });
}

void FakeServer::getAdditionalData()
{
    bool error = (qrand() % 100 == 1);

    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onAdditionalDataError() : onAdditionalData(); });
}

void FakeServer::getRelations(const QString &packageId)
{
    bool error = (qrand() % 100 == 1);

    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onRelationsError() : onRelations({packageId, }); });
}

void FakeServer::getRelations(const QStringList &packageIds)
{
    bool error = (qrand() % 100 == 1);

    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onRelationsError() : onRelations(packageIds); });
}

void FakeServer::addRelation(const QString &packageId, const QStringList &relatedIds)
{
    bool error = (qrand() % 100 == 1);

    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onRelationSaveError() : onRelationSaved(packageId, relatedIds); });
}

void FakeServer::getEntitiesInfo(int count, const QDateTime &from, const QString &keyword)
{
    bool error = (qrand() % 100 == 1);

    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onEntityError() : onEntityInfo(count, from, keyword); });
}

void FakeServer::getEntitiesInfo(const QDateTime &from, const QDateTime &to, const QString &keyword)
{
    bool error = (qrand() % 100 == 1);

    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onEntityError() : onEntityInfo(from, to, keyword); });
}

void FakeServer::getEntities(const QStringList &packageIds)
{
    bool error = (qrand() % 100 == 1);

    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onEntityError() : onEntity(packageIds); });
}

void FakeServer::getEntity(const QString &packageId)
{
    bool error = (qrand() % 100 == 1);

    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onEntityError() : onEntity({ packageId, }); });
}

void FakeServer::getEntityId(const QByteArray &codeData)
{
    bool error = (qrand() % 100 == 1);

    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onEntityError() : onEntityId(codeData); });
}

void FakeServer::putEntityAction(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties, const QByteArray &codeData)
{
    bool error = (qrand() % 100 == 1);

    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onEntitySaveError() : onEntitySaved(packageId, action, timestamp, properties, codeData); });
}

void FakeServer::putEntityAction(const QByteArray &codeData, const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties)
{
    bool error = (qrand() % 100 == 1);

    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onEntitySaveError() : onEntitySaved(QString{}, action, timestamp, properties, codeData); });
}

void FakeServer::postNewEntity(const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties, const QByteArray &codeData)
{
    bool error = (qrand() % 100 == 1);

    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onEntitySaveError() : onEntitySaved(codeData, action, timestamp, properties); });
}

void FakeServer::getCreatedHarvestIds()
{
    bool error = (qrand() % 100 == 1);

    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onCreatedHarvestIdsError() : onCreatedHarvestIds(); });
}

void FakeServer::getUnusedLotIds()
{
    bool error = (qrand() % 100 == 1);

    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onUnusedLotIdsError() : onUnusedLotIds(); });
}

void FakeServer::postUnusedLotId()
{
    bool error = (qrand() % 100 == 1);

    QTimer::singleShot(randomWaitTime(), this, [=]() { error ? onUnusedLotIdCreationError() : onUnusedLotIdCreated(); });
}

int FakeServer::randomWaitTime()
{
    return qrand() % 1000;
}

void FakeServer::onPingError()
{
    const auto error = QNetworkReply::HostNotFoundError;

    emit connectionState(error);
    emit pingError(error);
}

void FakeServer::onPingSuccess()
{
    emit connectionState(QNetworkReply::NoError);
    emit pingSuccess();
}

void FakeServer::onLoginError()
{
    const auto error = QNetworkReply::HostNotFoundError;

    emit connectionState(error);

    m_currentUserType = Enums::UserType::Annonymous;
    m_currentCooperativeId = QString{};

    emit loginError(error);
}

void FakeServer::onLogin(const QString &email, const QString &password)
{
    emit connectionState(QNetworkReply::NoError);

    if (m_populator.checkLogin(email, password)) {
        auto userData = m_populator.generateUserData(email);
        m_currentUserType = m_populator.userType(email);
        m_currentCooperativeId = userData.value(Tags::cooperativeId).toString();

        emit loginFinished(QJsonDocument::fromVariant(userData));
    } else {
        m_currentUserType = Enums::UserType::Annonymous;
        m_currentCooperativeId = QString{};

        emit loginError(QNetworkReply::AuthenticationRequiredError);
    }
}

void FakeServer::onAdditionalDataError()
{
    const auto error = QNetworkReply::HostNotFoundError;

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

void FakeServer::onRelationsError()
{
    const auto error = QNetworkReply::HostNotFoundError;

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
                                               { Tags::id, data.first },
                                               { Tags::ids, data.second }
                                           });
    });
    emit relationsLoaded(relationsArray);
}

void FakeServer::onRelationSaveError()
{
    const auto error = QNetworkReply::HostNotFoundError;

    emit connectionState(error);
    emit relationsSaveError(error);
}

void FakeServer::onRelationSaved(const QString &packageId, const QStringList &relatedIds)
{
    emit connectionState(QNetworkReply::NoError);

    m_populator.addPackageRelation(packageId, relatedIds);
    emit relationsSaved(packageId);
}

void FakeServer::onEntityError()
{
    const auto error = QNetworkReply::HostNotFoundError;

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

void FakeServer::onEntityIdError()
{
    const auto error = QNetworkReply::HostNotFoundError;

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

void FakeServer::onEntitySaveError()
{
    const auto error = QNetworkReply::HostNotFoundError;

    emit connectionState(error);
    emit entitySaveError(error);
}

void FakeServer::onEntitySaved(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties, const QByteArray &codeData)
{
    emit connectionState(QNetworkReply::NoError);

    auto result = m_populator.addAction(packageId, action, timestamp, properties, codeData, m_currentUserType, m_currentCooperativeId);
    if (!result) {
        emit entitySaveError(-1);
    } else {
        emit entitySaved(packageId, action);
    }
}

void FakeServer::onEntitySaved(const QByteArray &codeData, const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties)
{
    emit connectionState(QNetworkReply::NoError);

    auto packageId = m_populator.addNewAction(action, timestamp, properties, codeData, m_currentUserType, m_currentCooperativeId);
    if (packageId.isEmpty()) {
        emit entitySaveError(-1);
    } else {
        emit entitySaved(packageId, action);
    }
}

void FakeServer::onCreatedHarvestIdsError()
{
    const auto error = QNetworkReply::HostNotFoundError;

    emit connectionState(error);
    emit createdHarvestIdsLoadError(error);
}

void FakeServer::onCreatedHarvestIds()
{
    emit connectionState(QNetworkReply::NoError);

    emit createdHarvestIdsLoaded(QJsonArray::fromVariantList(m_populator.createdHarvestIds(m_currentCooperativeId)) );
}

void FakeServer::onUnusedLotIdsError()
{
    const auto error = QNetworkReply::HostNotFoundError;

    emit connectionState(error);
    emit unusedLotIdsLoadError(error);
}

void FakeServer::onUnusedLotIds()
{
    emit connectionState(QNetworkReply::NoError);

    emit unusedLotIdsLoaded(QJsonArray::fromVariantList(m_populator.unusedLotIds(m_currentCooperativeId)) );
}

void FakeServer::onUnusedLotIdCreationError()
{
    const auto error = QNetworkReply::HostNotFoundError;

    emit connectionState(error);
    emit unusedLotIdCreateError(error);
}

void FakeServer::onUnusedLotIdCreated()
{
    emit connectionState(QNetworkReply::NoError);

    emit unusedLotIdCreated(m_populator.createUnusedLotId(m_currentCooperativeId));
}
