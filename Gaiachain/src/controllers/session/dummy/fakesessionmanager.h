#ifndef FAKESESSIONMANAGER_H
#define FAKESESSIONMANAGER_H

#include "../abstractsessionmanager.h"
#include "fakedatapopulator.h"

class FakeSessionManager : public AbstractSessionManager
{
    Q_OBJECT

public:
    explicit FakeSessionManager(QObject *parent = nullptr);

    Q_INVOKABLE void login(const QString &email, const QString &password) override;

    Q_INVOKABLE void getAdditionalData() override;

    Q_INVOKABLE void getRelations(const QString &id) override;
    Q_INVOKABLE void addRelation(const QString &id, const QStringList &ids) override;

    Q_INVOKABLE void getEntitiesInfo(int count, const QDateTime &from = {}) override;
    Q_INVOKABLE void getEntitiesInfo(const QDateTime &to, const QDateTime &from = {}) override;
    Q_INVOKABLE void getEntities(const QStringList &ids) override;
    Q_INVOKABLE void getEntity(const QString &id) override;
    Q_INVOKABLE void getEntitId(const QByteArray &codeData) override;

    Q_INVOKABLE void putEntityAction(const QString &id, const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                                             const QVariantMap &properties, const QByteArray &codeData = {}) override;
    Q_INVOKABLE void putEntity(const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                                       const QVariantMap &properties, const QByteArray &codeData) override;

    Q_INVOKABLE void getUnusedLotIds() override;
    Q_INVOKABLE void createUnusedLotId() override;

private:
    static const int sc_firstHarvestShift = 90;    // more or less a quarter

    FakeDataPopulator m_populator;
    Enums::UserType m_currentUserType;
    QString m_currentCooperativeId;

    void getAllRelations() override;
    void getAllEntities() override;

    int randomWaitTime();

    void onLoginError();
    void onLogin(const QString &email, const QString &password);

    void onAdditionalDataError();
    void onAdditionalData();

    void onRelationsError();
    void onRelationsAll();
    void onRelationsSingle(const QString &packageId);

    void onRelationSaveError(const QString &packageId);
    void onRelationSaved(const QString &packageId, const QStringList &relatedIds);

    void onEntityError();
    void onEntityAll();
    void onEntityMultiple(const QStringList &packagesId);
    void onEntitySingle(const QString &packageId);
    void onEntityId(const QByteArray &codeData);

    void onEntitySaveError(const QString &packageId);
    void onEntitySaved(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                       const QVariantMap &properties, const QByteArray &codeData);
    void onEntitySaved(const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                       const QVariantMap &properties, const QByteArray &codeData);

    void onUnusedLotIdsError();
    void onUnusedLotIds();

    void onUnusedLotIdCreationError();
    void onUnusedLotIdCreated();
};

#endif // FAKESESSIONMANAGER_H
