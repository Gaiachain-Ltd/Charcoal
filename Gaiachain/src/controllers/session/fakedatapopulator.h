#ifndef FAKEDATAPOPULATOR_H
#define FAKEDATAPOPULATOR_H

#include <QObject>
#include <QHash>
#include <QVector>
#include <QVariantMap>

#include "../../common/enums.h"
#include "../../common/location.h"

class EventModel;
class ShipmentModel;

class FakeDataPopulator : public QObject
{
    Q_OBJECT
public:
    explicit FakeDataPopulator();

    bool checkLogin(const QString &email, const QString &password);
    QVariantMap generateUserData(const QString &email);

    void populateFakeData(const int count, const QDateTime &startDate);

    QMap<QVariant, QVariantList> getShipmentsHistory() const;
    bool canAddAction(const QString &shipmentId, const Enums::SupplyChainAction &action, const Enums::ActionProgress &actionProgress) const;
    bool addAction(const QString &shipmentId, const Enums::SupplyChainAction &action, const Enums::ActionProgress &actionProgress);

private:
    static const int ID_LENGTH = 12;

    static const QString sc_loginPassword;
    static const QHash<QString, Enums::UserType> sc_usersLogin;
    static const QHash<Enums::UserType, int> sc_companiesCount;
    static const QHash<Enums::UserType, QStringList> sc_companiesName;
    static const QHash<Enums::UserType, QList<Location>> sc_companiesLocation;

    static const QVector<Enums::SupplyChainAction> sc_availableActions;
    static const QVector<Enums::ActionProgress> sc_availableActionProgresses;
    static const QHash<Enums::SupplyChainAction, Enums::UserType> sc_actionsUser;

    QMap<QVariant, QVariantList> m_shipmentsHistory;

    QPair<QString, Location> randomCompany(const Enums::UserType &userType) const;
    Location companyLocation(const Enums::UserType &userType, const QString &companyName) const;

    QString generateRandomId() const;
    QStringList generateRandomIdList(const int count) const;

    QPair<Enums::SupplyChainAction, Enums::ActionProgress> getNextAction(const QString &shipmentId) const;
};

#endif // FAKEDATAPOPULATOR_H
