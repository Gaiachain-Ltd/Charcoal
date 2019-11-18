#ifndef DATAGLOBALS_H
#define DATAGLOBALS_H

#include <QObject>
#include <QString>
#include <QMetaEnum>
#include <QDate>

#include "../common/enums.h"

class DataGlobals : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList availableUserTypes READ availableUserTypesQml CONSTANT)
    Q_PROPERTY(QVariantList availablePlaceTypes READ availablePlaceTypesQml CONSTANT)
    Q_PROPERTY(QVariantList availablePackageTypes READ availablePackageTypesQml CONSTANT)
    Q_PROPERTY(QVariantList availableActions READ availableActionsQml CONSTANT)

public:
    static DataGlobals *instance();

    Q_INVOKABLE static Enums::PackageType packageType(const Enums::SupplyChainAction &action);
    Q_INVOKABLE static Enums::PlaceType placeType(const Enums::SupplyChainAction &action);
    Q_INVOKABLE static Enums::PlaceType userPlaceType(const Enums::UserType &user);
    static QList<Enums::SupplyChainAction> userActions(const Enums::UserType &user);
    static QList<Enums::SupplyChainAction> packageActions(const Enums::PackageType &type);
    static Enums::SupplyChainAction packageTypeCreationAction(const Enums::PackageType &type);

    static QList<Enums::UserType> availableUserTypes();
    static QList<Enums::PlaceType> availablePlaceTypes();
    static QList<Enums::PackageType> availablePackageTypes();
    static QList<Enums::SupplyChainAction> availableActions();

    // qmls
    Q_INVOKABLE static QVariantList userActionsQml(const Enums::UserType &user);

    Q_INVOKABLE static QVariantList availableUserTypesQml();
    Q_INVOKABLE static QVariantList availablePlaceTypesQml();
    Q_INVOKABLE static QVariantList availablePackageTypesQml();
    Q_INVOKABLE static QVariantList availableActionsQml();

private:
    DataGlobals();

    static const QHash<Enums::SupplyChainAction, Enums::PackageType> sc_supplyChainActionPackageType;
    static const QHash<Enums::SupplyChainAction, Enums::PlaceType> sc_supplyChainActionPlaceType;
    static const QHash<Enums::UserType, QList<Enums::SupplyChainAction>> sc_supplyChainActionPerUser;
    static const QHash<Enums::UserType, Enums::PlaceType> sc_userPlaceType;
    static const QHash<Enums::PackageType, Enums::SupplyChainAction> sc_packageTypeCreationAction;
};

#endif // DATAGLOBALS_H
