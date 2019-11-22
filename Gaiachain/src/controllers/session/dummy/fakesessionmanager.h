#ifndef FAKESESSIONMANAGER_H
#define FAKESESSIONMANAGER_H

#include "fakeserver.h"

class FakeSessionManager : public AbstractSessionManager
{
    Q_OBJECT

public:
    explicit FakeSessionManager(QObject *parent = nullptr);

    Q_INVOKABLE void ping() override;
    Q_INVOKABLE void login(const QString &email, const QString &password) override;

    Q_INVOKABLE void getAdditionalData() override;

    Q_INVOKABLE void getRelations(const QString &packageId) override;
    Q_INVOKABLE void getRelations(const QStringList &packageIds) override;
    Q_INVOKABLE void addRelation(const QString &packageId, const QStringList &relatedIds) override;

    Q_INVOKABLE void getEntitiesInfo(int count, const QDateTime &from, const QString &keyword) override;
    Q_INVOKABLE void getEntitiesInfo(const QDateTime &from, const QDateTime &to, const QString &keyword) override;
    Q_INVOKABLE void getEntities(const QStringList &packageIds) override;
    Q_INVOKABLE void getEntity(const QString &packageId) override;
    Q_INVOKABLE void getEntityId(const QByteArray &codeData) override;

    Q_INVOKABLE void putEntityAction(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                                             const QVariantMap &properties, const QByteArray &codeData = {}) override;
    Q_INVOKABLE void putEntityAction(const QByteArray &codeData, const Enums::SupplyChainAction &action,
                                     const QDateTime &timestamp, const QVariantMap &properties) override;
    Q_INVOKABLE void postNewEntity(const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                                   const QVariantMap &properties, const QByteArray &codeData) override;

    Q_INVOKABLE void getCreatedHarvestIds() override;

    Q_INVOKABLE void getUnusedLotIds() override;
    Q_INVOKABLE void postUnusedLotId() override;

private:
    FakeServer m_fakeServer;
};

#endif // FAKESESSIONMANAGER_H
