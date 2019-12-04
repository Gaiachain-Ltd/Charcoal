#ifndef FAKESERVER_H
#define FAKESERVER_H

#include "../abstractsessionmanager.h"
#include "fakedatapopulator.h"

#include <QThread>

class FakeServer : public QObject
{
    Q_OBJECT

public:
    ~FakeServer();

    static FakeServer &instance();

public slots:
    void ping();
    void login(const QString &login, const QString &password);

    void getAdditionalData();

    void getRelations(const QString &packageId);
    void getRelations(const QStringList &packageIds);
    void addRelation(const QString &packageId, const QStringList &relatedIds);

    void getEntitiesInfo(int count, const QDateTime &from, const QString &);
    void getEntitiesInfo(const QDateTime &from, const QDateTime &to, const QString &);
    void getLastActionEntitiesInfo(const Enums::SupplyChainAction &lastAction);

    void getEntities(const QStringList &packageIds);
    void getEntity(const QString &packageId);
    void getEntityId(const QByteArray &codeData);

    void putEntityAction(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                         const QVariantMap &properties, const QByteArray &codeData = {});
    void putEntityAction(const QByteArray &codeData, const Enums::SupplyChainAction &action,
                         const QDateTime &timestamp, const QVariantMap &properties);
    void postNewEntity(const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                       const QVariantMap &properties, const QByteArray &codeData);

    void getUnusedLotIds();
    void postUnusedLotId();

signals:
    void connectionState(const QNetworkReply::NetworkError &errorCode) const;
    void requestFinished() const;

    void pingError(const QNetworkReply::NetworkError &code) const;
    void pingSuccess() const;

    void loginError(const QString &login, const QNetworkReply::NetworkError &code) const;
    void loginFinished(const QString &login, const QJsonObject &userDataObj) const;

    void additionalDataLoadError(const QNetworkReply::NetworkError &code) const;
    void additionalDataLoaded(const QJsonObject &data) const;

    void relationsLoadError(const QNetworkReply::NetworkError &code) const;
    void relationsLoaded(const QJsonArray &relations) const;
    void relationsSaveError(const QString &packageId, const QNetworkReply::NetworkError &code) const;
    void relationsSaved(const QString &packageId) const;

    void entitiesLoadError(const QNetworkReply::NetworkError &code) const;
    void entitiesInfoLoaded(const QJsonArray &entitiesInfo) const;
    void entitiesLoaded(const QJsonArray &entities) const;

    void entityIdLoadError(const QNetworkReply::NetworkError &code) const;
    void entityIdLoaded(const QString &packageId) const;

    void entitySaveError(const QString &packageId, const Enums::SupplyChainAction &action, const QNetworkReply::NetworkError &code) const;
    void entitySaved(const QString &packageId, const Enums::SupplyChainAction &action) const;

    void unusedLotIdsLoadError(const QNetworkReply::NetworkError &code) const;
    void unusedLotIdsLoaded(const QJsonArray &ids) const;
    void unusedLotIdCreateError(const QNetworkReply::NetworkError &code) const;
    void unusedLotIdCreated(const QString &id) const;

private:
    explicit FakeServer(QObject *parent = nullptr);

    QThread m_thread;

    static const int sc_firstHarvestShift = 90;    // more or less a quarter

    FakeDataPopulator m_populator;
    Enums::UserType m_currentUserType;
    QString m_currentCooperativeId;

    bool isError(const QNetworkReply::NetworkError &error) const;
    QNetworkReply::NetworkError randomError() const;
    int randomWaitTime() const;

    void onPingError(const QNetworkReply::NetworkError &error);
    void onPingSuccess();

    void onLoginError(const QString &login, const QNetworkReply::NetworkError &error);
    void onLogin(const QString &login, const QString &password);

    void onAdditionalDataError(const QNetworkReply::NetworkError &error);
    void onAdditionalData();

    void onRelationsError(const QNetworkReply::NetworkError &error);
    void onRelations(const QStringList &packagesIds);

    void onRelationSaveError(const QString &packageId, const QNetworkReply::NetworkError &error);
    void onRelationSaved(const QString &packageId, const QStringList &relatedIds);

    void onEntityError(const QNetworkReply::NetworkError &error);

    void onEntityInfo(int count, const QDateTime &from, const QString &keyword);
    void onEntityInfo(const QDateTime &from, const QDateTime &to, const QString &keyword);
    void onLastActionEntitiesInfo(const Enums::SupplyChainAction &lastAction);

    void onEntityAll();
    void onEntity(const QStringList &packagesId);

    void onEntityIdError(const QNetworkReply::NetworkError &error);
    void onEntityId(const QByteArray &codeData);

    void onEntitySaveError(const QString &packageId, const Enums::SupplyChainAction &action, const QNetworkReply::NetworkError &error);
    void onEntitySaved(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                       const QVariantMap &properties, const QByteArray &codeData);
    void onEntitySaved(const QByteArray &codeData, const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                       const QVariantMap &properties);

    void onUnusedLotIdsError(const QNetworkReply::NetworkError &error);
    void onUnusedLotIds();

    void onUnusedLotIdCreationError(const QNetworkReply::NetworkError &error);
    void onUnusedLotIdCreated();
};

#endif // FAKESERVER_H
