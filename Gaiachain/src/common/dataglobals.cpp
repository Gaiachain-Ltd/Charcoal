#include "dataglobals.h"

#include "../helpers/utility.h"

const QList<Enums::SupplyChainAction> DataGlobals::sc_supplyChainOfflineActions = {
    Enums::SupplyChainAction::Harvest,
    Enums::SupplyChainAction::GrainProcessing
};

const QHash<Enums::SupplyChainAction, Enums::PackageType> DataGlobals::sc_supplyChainActionPackageType = {
    { Enums::SupplyChainAction::Harvest, Enums::PackageType::Harvest },
    { Enums::SupplyChainAction::GrainProcessing, Enums::PackageType::Harvest },
    { Enums::SupplyChainAction::SectionReception, Enums::PackageType::Harvest },
    { Enums::SupplyChainAction::Bagging, Enums::PackageType::Sac },
    { Enums::SupplyChainAction::LotCreation, Enums::PackageType::Lot },
    { Enums::SupplyChainAction::WarehouseTransport, Enums::PackageType::Lot},
    { Enums::SupplyChainAction::ExportReception, Enums::PackageType::Lot }
};

const QHash<Enums::PackageType, Enums::SupplyChainAction> DataGlobals::sc_packageTypeCreationAction = {
    { Enums::PackageType::Harvest, Enums::SupplyChainAction::Harvest},
    { Enums::PackageType::Sac, Enums::SupplyChainAction::Bagging },
    { Enums::PackageType::Lot, Enums::SupplyChainAction::LotCreation }
};

const QHash<Enums::UserType, QList<Enums::SupplyChainAction>> DataGlobals::sc_supplyChainActionPerUser = {
    { Enums::UserType::SuperUser, QList<Enums::SupplyChainAction>{ Enums::SupplyChainAction::Harvest,
                                                                   Enums::SupplyChainAction::GrainProcessing,
                                                                   Enums::SupplyChainAction::SectionReception,
                                                                   Enums::SupplyChainAction::Bagging,
                                                                   Enums::SupplyChainAction::LotCreation,
                                                                   Enums::SupplyChainAction::WarehouseTransport,
                                                                   Enums::SupplyChainAction::ExportReception, } },
    { Enums::UserType::Inspector, QList<Enums::SupplyChainAction>{ Enums::SupplyChainAction::Harvest,
                                                                   Enums::SupplyChainAction::GrainProcessing } },
    { Enums::UserType::PCA, QList<Enums::SupplyChainAction>{ Enums::SupplyChainAction::SectionReception,
                                                             Enums::SupplyChainAction::Bagging,
                                                             Enums::SupplyChainAction::LotCreation } },
    { Enums::UserType::Warehouseman, QList<Enums::SupplyChainAction>{ Enums::SupplyChainAction::SectionReception,
                                                                      Enums::SupplyChainAction::Bagging,
                                                                      Enums::SupplyChainAction::LotCreation,
                                                                      Enums::SupplyChainAction::WarehouseTransport } },
    { Enums::UserType::CooperativeRepresentative, QList<Enums::SupplyChainAction>{ Enums::SupplyChainAction::ExportReception } }
};

DataGlobals &DataGlobals::instance()
{
    static DataGlobals dg;
    return dg;
}

Enums::PackageType DataGlobals::packageType(const Enums::SupplyChainAction &action)
{
    return sc_supplyChainActionPackageType.value(action, Enums::PackageType::Unknown);
}

QList<Enums::SupplyChainAction> DataGlobals::userActions(const Enums::UserType &user)
{
    return sc_supplyChainActionPerUser.value(user);
}

QList<Enums::SupplyChainAction> DataGlobals::packageActions(const Enums::PackageType &type)
{
    auto packageActions = QList<Enums::SupplyChainAction>{};
    std::copy_if(sc_supplyChainActionPackageType.keyBegin(), sc_supplyChainActionPackageType.keyEnd(),
                 std::back_inserter(packageActions),
                 [&type](const auto &action) { return (sc_supplyChainActionPackageType.value(action) == type); });

    return packageActions;
}

Enums::SupplyChainAction DataGlobals::packageTypeCreationAction(const Enums::PackageType &type)
{
    return sc_packageTypeCreationAction.value(type);
}

QList<Enums::UserType> DataGlobals::availableUserTypes()
{
    static const auto availableList = Utility::generateEnumValues(Enums::UserType::Annonymous, Enums::UserType::UserTypeCount);
    return availableList;
}

QList<Enums::PackageType> DataGlobals::availablePackageTypes()
{
    static const auto availableList = Utility::generateEnumValues(Enums::PackageType::Unknown, Enums::PackageType::PackageTypeCount);
    return availableList;
}

QList<Enums::SupplyChainAction> DataGlobals::availableActions()
{
    static const auto availableList = Utility::generateEnumValues(Enums::SupplyChainAction::Unknown, Enums::SupplyChainAction::SupplyChainActionCount);
    return availableList;
}

QList<Enums::SupplyChainAction> DataGlobals::availableOfflineActions()
{
    return sc_supplyChainOfflineActions;
}

QVariantList DataGlobals::userActionsQml(const Enums::UserType &user)
{
    return Utility::toVariantList(userActions(user), QMetaType::Int);
}

QVariantList DataGlobals::availableUserTypesQml()
{
    static auto variantList = Utility::toVariantList(availableUserTypes(), QMetaType::Int);
    return variantList;
}

QVariantList DataGlobals::availablePackageTypesQml()
{
    static auto variantList = Utility::toVariantList(availablePackageTypes(), QMetaType::Int);
    return variantList;
}

QVariantList DataGlobals::availableActionsQml()
{
    static auto variantList = Utility::toVariantList(availableActions(), QMetaType::Int);
    return variantList;
}

QVariantList DataGlobals::availableOfflineActionsQml()
{
    static auto variantList = Utility::toVariantList(availableOfflineActions(), QMetaType::Int);
    return variantList;
}

DataGlobals::DataGlobals()
{}
