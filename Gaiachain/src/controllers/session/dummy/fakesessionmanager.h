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

    Q_INVOKABLE void getEntity() override;
    Q_INVOKABLE void getEntity(const QString &id) override;
    Q_INVOKABLE void putEntity(const QString &id, const Enums::SupplyChainAction &action, const QVariantMap &properties) override;

private:
    static const int sc_firstHarvestShift = 90;    // more or less a quarter

    FakeDataPopulator m_populator;
    Enums::UserType m_currentUserType;

    int randomWaitTime();

    void onLogin(const QString &email, const QString &password);
    void onLoginError();

    void onEntityError();
    void onEntityAll();
    void onEntitySingle(const QString &packageId);

    void onEntitySaveError(const QString &packageId);
    void onEntitySaved(const QString &packageId, const Enums::SupplyChainAction &action, const QVariantMap &properties);
};

#endif // FAKESESSIONMANAGER_H
