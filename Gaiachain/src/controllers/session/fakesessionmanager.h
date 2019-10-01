#ifndef FAKESESSIONMANAGER_H
#define FAKESESSIONMANAGER_H

#include "abstractsessionmanager.h"
#include "fakedatapopulator.h"

class FakeSessionManager : public AbstractSessionManager
{
    Q_OBJECT

public:
    explicit FakeSessionManager(QObject *parent = nullptr);

    Q_INVOKABLE void login(const QString &email, const QString &password) override;

    Q_INVOKABLE void getEntity() override;
    Q_INVOKABLE void getEntity(const QString &id) override;
    Q_INVOKABLE void putEntity(const QString &id, const Enums::SupplyChainAction &action, const Enums::ActionProgress &actionProgress) override;

private:
    static const int sc_initialShipmentsCount = 40;
    static const int sc_firstShipmentShift = 30;    // more or less a month

    FakeDataPopulator m_populator;

    int randomWaitTime();

    void onLogin(const QString &email, const QString &password);
    void onLoginError();

    void onEntityError();
    void onEntityAll();
    void onEntitySingle(const QString &shipmentId);

    void onEntitySaveError(const QString &id);
    void onEntitySaved(const QString &id, const Enums::SupplyChainAction &action, const Enums::ActionProgress &actionProgress);
};

#endif // FAKESESSIONMANAGER_H
