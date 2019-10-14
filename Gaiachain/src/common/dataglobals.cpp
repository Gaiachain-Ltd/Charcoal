#include "dataglobals.h"

const QHash<Enums::UserType, QString> DataGlobals::sc_userTypeStrings = {
    {Enums::UserType::Producer, QStringLiteral("PRODUCER") },
    {Enums::UserType::BaggingPerson, QStringLiteral("BAGGING") },
    {Enums::UserType::Storekeeper, QStringLiteral("STOREKEEPER") },
    {Enums::UserType::Exporter, QStringLiteral("EXPORTER") },
    {Enums::UserType::NotLoggedUser, QString()}
};
const QHash<Enums::SupplyChainAction, QString> DataGlobals::sc_supplyChainActionStrings = {
    {Enums::SupplyChainAction::Harvest, QStringLiteral("HARVEST") },
    {Enums::SupplyChainAction::Fermentation, QStringLiteral("FERMENTATION") },
    {Enums::SupplyChainAction::Bagging, QStringLiteral("BAGGING") },
    {Enums::SupplyChainAction::StorageArrival, QStringLiteral("STORAGE_ARRIVAL") },
    {Enums::SupplyChainAction::StorageLot, QStringLiteral("STORAGE_LOT") },
    {Enums::SupplyChainAction::Transport, QStringLiteral("TRANSPORT") },
    {Enums::SupplyChainAction::Reception, QStringLiteral("RECEPTION") }
};
const QHash<Enums::ActionProgress, QString> DataGlobals::sc_actionProgressStrings = {
    {Enums::ActionProgress::Started, QStringLiteral("ACTION_STARTED") },
    {Enums::ActionProgress::Finished, QStringLiteral("ACTION_FINISHED") }
};

const QHash<Enums::SupplyChainAction, Enums::PackageType> DataGlobals::sc_supplyChainActionPackageType = {
    {Enums::SupplyChainAction::Harvest, Enums::PackageType::Harvest },
    {Enums::SupplyChainAction::Fermentation, Enums::PackageType::Harvest },
    {Enums::SupplyChainAction::Bagging, Enums::PackageType::Sac },
    {Enums::SupplyChainAction::StorageArrival, Enums::PackageType::Sac },
    {Enums::SupplyChainAction::StorageLot, Enums::PackageType::Lot },
    {Enums::SupplyChainAction::Transport, Enums::PackageType::Lot},
    {Enums::SupplyChainAction::Reception, Enums::PackageType::Lot }
};

DataGlobals *DataGlobals::instance()
{
    static DataGlobals dg;
    return &dg;
}

Enums::UserType DataGlobals::userTypeFromString(const QString &text)
{
    return sc_userTypeStrings.key(text);
}

QString DataGlobals::userTypeToString(const Enums::UserType &userType)
{
    return sc_userTypeStrings.value(userType);
}

Enums::SupplyChainAction DataGlobals::supplyChainActionFromString(const QString &text)
{
    return sc_supplyChainActionStrings.key(text);
}

QString DataGlobals::supplyChainActionToString(const Enums::SupplyChainAction &action)
{
    return sc_supplyChainActionStrings.value(action);
}

Enums::ActionProgress DataGlobals::actionProgressFromString(const QString &text)
{
    return sc_actionProgressStrings.key(text);
}

QString DataGlobals::actionProgressToString(const Enums::ActionProgress &actionProgress)
{
    return sc_actionProgressStrings.value(actionProgress);
}

Enums::PackageType DataGlobals::packageType(const Enums::SupplyChainAction &action)
{
    return sc_supplyChainActionPackageType.value(action, Enums::PackageType::Unknown);
}

DataGlobals::DataGlobals()
{}
