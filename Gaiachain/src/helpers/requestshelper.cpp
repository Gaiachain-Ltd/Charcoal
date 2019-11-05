#include "requestshelper.h"

#include "../common/tags.h"

const QHash<Enums::UserType, QString> RequestsHelper::sc_userTypeStrings = {
    { Enums::UserType::SuperUser, StaticValues::userSuperUser },
    { Enums::UserType::Inspector, StaticValues::userInspector },
    { Enums::UserType::PCA, StaticValues::userPca },
    { Enums::UserType::Warehouseman, StaticValues::userWarehouseman },
    { Enums::UserType::CooperativeRepresentative, StaticValues::userCooperativeRepresentative }
};
const QHash<Enums::PackageType, QString> RequestsHelper::sc_packageTypeStrings = {
    { Enums::PackageType::Harvest, StaticValues::packageHarvest },
    { Enums::PackageType::Sac, StaticValues::packageSac },
    { Enums::PackageType::Lot, StaticValues::packageLot }
};
const QHash<Enums::SupplyChainAction, QString> RequestsHelper::sc_supplyChainActionStrings = {
    { Enums::SupplyChainAction::Harvest, StaticValues::actionHarvest },
    { Enums::SupplyChainAction::GrainProcessing, StaticValues::actionGrainProcessing },
    { Enums::SupplyChainAction::SectionReception, StaticValues::actionSectionReception },
    { Enums::SupplyChainAction::Bagging, StaticValues::actionBagging },
    { Enums::SupplyChainAction::LotCreation, StaticValues::actionLotCreation },
    { Enums::SupplyChainAction::WarehouseTransport, StaticValues::actionWarehouseTransport },
    { Enums::SupplyChainAction::ExportReception, StaticValues::actionExportReception }
};

Enums::UserType RequestsHelper::userTypeFromString(const QString &text)
{
    return sc_userTypeStrings.key(text);
}

QString RequestsHelper::userTypeToString(const Enums::UserType &userType)
{
    return sc_userTypeStrings.value(userType);
}

Enums::PackageType RequestsHelper::packageTypeFromString(const QString &text)
{
    return sc_packageTypeStrings.key(text);
}

QString RequestsHelper::packageTypeToString(const Enums::PackageType &packageType)
{
    return sc_packageTypeStrings.value(packageType);
}

Enums::SupplyChainAction RequestsHelper::supplyChainActionFromString(const QString &text)
{
    return sc_supplyChainActionStrings.key(text);
}

QString RequestsHelper::supplyChainActionToString(const Enums::SupplyChainAction &action)
{
    return sc_supplyChainActionStrings.value(action);
}

RequestsHelper::RequestsHelper()
{}
