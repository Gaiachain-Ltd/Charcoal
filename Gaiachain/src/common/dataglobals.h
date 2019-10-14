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
public:
    static DataGlobals *instance();

    static Enums::UserType userTypeFromString(const QString &text);
    static QString userTypeToString(const Enums::UserType &userType);

    static Enums::SupplyChainAction supplyChainActionFromString(const QString &text);
    static QString supplyChainActionToString(const Enums::SupplyChainAction &action);

    static Enums::ActionProgress actionProgressFromString(const QString &text);
    static QString actionProgressToString(const Enums::ActionProgress &actionProgress);

    Q_INVOKABLE static Enums::PackageType packageType(const Enums::SupplyChainAction &action);

private:
    DataGlobals();

    static const QHash<Enums::UserType, QString> sc_userTypeStrings;
    static const QHash<Enums::SupplyChainAction, QString> sc_supplyChainActionStrings;
    static const QHash<Enums::ActionProgress, QString> sc_actionProgressStrings;

    static const QHash<Enums::SupplyChainAction, Enums::PackageType> sc_supplyChainActionPackageType;
};

#endif // DATAGLOBALS_H
