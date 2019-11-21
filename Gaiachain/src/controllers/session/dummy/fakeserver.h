#ifndef FAKESERVER_H
#define FAKESERVER_H

#include "../abstractsessionmanager.h"
#include "fakedatapopulator.h"

#include <QThread>

class FakeServer : public QObject
{
    Q_OBJECT

public:
    explicit FakeServer(QObject *parent = nullptr);

public slots:
    void ping();
    void login(const QString &email, const QString &password);

    void getAdditionalData();

    void getRelations(const QString &packageId);
    void getRelations(const QStringList &packageIds);
    void addRelation(const QString &packageId, const QStringList &relatedIds);

    void getEntitiesInfo(int count, const QDateTime &from);
    void getEntitiesInfo(const QDateTime &from, const QDateTime &to);
    void getEntities(const QStringList &packageIds);
    void getEntity(const QString &packageId);
    void getEntityId(const QByteArray &codeData);

    void putEntityAction(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                         const QVariantMap &properties, const QByteArray &codeData = {});
    void putEntityAction(const QByteArray &codeData, const Enums::SupplyChainAction &action,
                         const QDateTime &timestamp, const QVariantMap &properties);
    void postNewEntity(const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                       const QVariantMap &properties, const QByteArray &codeData);

    void getCreatedHarvestIds();

    void getUnusedLotIds();
    void postUnusedLotId();

signals:
    void connectionState(const int &errorCode);

    void pingError(const int &code) const;
    void pingSuccess() const;

    void loginError(const int &code) const;
    void loginFinished(const QJsonDocument &doc) const;

    void additionalDataLoadError(const int &code) const;
    void additionalDataLoaded(const QJsonObject &data) const;

    void relationsLoadError(const int &code) const;
    void relationsLoaded(const QJsonArray &relations) const;
    void relationsSaveError(const int &code) const;
    void relationsSaved(const QString &packageId) const;

    void entitiesLoadError(const int &code) const;
    void entitiesInfoLoaded(const QJsonArray &entitiesInfo) const;
    void entitiesLoaded(const QJsonArray &entities) const;

    void entityIdLoadError(const int &code) const;
    void entityIdLoaded(const QString &packageId) const;

    void entitySaveError(const int &code) const;
    void entitySaved(const QString &packageId, const Enums::SupplyChainAction &action) const;

    void createdHarvestIdsLoadError(const int &code) const;
    void createdHarvestIdsLoaded(const QJsonArray &ids) const;

    void unusedLotIdsLoadError(const int &code) const;
    void unusedLotIdsLoaded(const QJsonArray &ids) const;
    void unusedLotIdCreateError(const int &code) const;
    void unusedLotIdCreated(const QString &id) const;

private:
    QThread m_thread;

    static const int sc_firstHarvestShift = 90;    // more or less a quarter

    FakeDataPopulator m_populator;
    Enums::UserType m_currentUserType;
    QString m_currentCooperativeId;

    int randomWaitTime();

    void onPingError();
    void onPingSuccess();

    void onLoginError();
    void onLogin(const QString &email, const QString &password);

    void onAdditionalDataError();
    void onAdditionalData();

    void onRelationsError();
    void onRelations(const QStringList &packagesIds);

    void onRelationSaveError();
    void onRelationSaved(const QString &packageId, const QStringList &relatedIds);

    void onEntityError();

    void onEntityInfo(int count, const QDateTime &from);
    void onEntityInfo(const QDateTime &from, const QDateTime &to);

    void onEntityAll();
    void onEntity(const QStringList &packagesId);

    void onEntityIdError();
    void onEntityId(const QByteArray &codeData);

    void onEntitySaveError();
    void onEntitySaved(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                       const QVariantMap &properties, const QByteArray &codeData);
    void onEntitySaved(const QByteArray &codeData, const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                       const QVariantMap &properties);

    void onCreatedHarvestIdsError();
    void onCreatedHarvestIds();

    void onUnusedLotIdsError();
    void onUnusedLotIds();

    void onUnusedLotIdCreationError();
    void onUnusedLotIdCreated();
};

#endif // FAKESERVER_H
