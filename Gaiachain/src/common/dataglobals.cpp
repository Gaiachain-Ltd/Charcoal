#include "dataglobals.h"

#include "../helpers/utility.h"

const QHash<Enums::SupplyChainAction, Enums::PackageType> DataGlobals::sc_supplyChainActionPackageType = {
    { Enums::SupplyChainAction::Harvest, Enums::PackageType::Harvest },
    { Enums::SupplyChainAction::GrainProcessing, Enums::PackageType::Harvest },
    { Enums::SupplyChainAction::SectionReception, Enums::PackageType::Harvest },
    { Enums::SupplyChainAction::Bagging, Enums::PackageType::Sac },
    { Enums::SupplyChainAction::LotCreation, Enums::PackageType::Lot },
    { Enums::SupplyChainAction::WarehouseTransport, Enums::PackageType::Lot},
    { Enums::SupplyChainAction::ExportReception, Enums::PackageType::Lot }
};

const QHash<Enums::SupplyChainAction, Enums::PlaceType> DataGlobals::sc_supplyChainActionPlaceType = {
    { Enums::SupplyChainAction::Harvest, Enums::PlaceType::Parcele },
    { Enums::SupplyChainAction::GrainProcessing, Enums::PlaceType::Parcele },
    { Enums::SupplyChainAction::SectionReception, Enums::PlaceType::Section },
    { Enums::SupplyChainAction::Bagging, Enums::PlaceType::Section },
    { Enums::SupplyChainAction::LotCreation, Enums::PlaceType::Section },
    { Enums::SupplyChainAction::WarehouseTransport, Enums::PlaceType::Section },
    { Enums::SupplyChainAction::ExportReception, Enums::PlaceType::CooperativeHQ }
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

const QHash<Enums::UserType, Enums::PlaceType> DataGlobals::sc_userPlaceType = {
    { Enums::UserType::SuperUser, Enums::PlaceType::Invalid },
    { Enums::UserType::Inspector, Enums::PlaceType::Parcele },
    { Enums::UserType::PCA, Enums::PlaceType::Section },
    { Enums::UserType::Warehouseman, Enums::PlaceType::Section },
    { Enums::UserType::CooperativeRepresentative, Enums::PlaceType::CooperativeHQ }
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

Enums::PlaceType DataGlobals::placeType(const Enums::SupplyChainAction &action)
{
    return sc_supplyChainActionPlaceType.value(action, Enums::PlaceType::Invalid);
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

Enums::PlaceType DataGlobals::userPlaceType(const Enums::UserType &user)
{
    return sc_userPlaceType.value(user);
}

QList<Enums::UserType> DataGlobals::availableUserTypes()
{
    static const auto availableList = Utility::generateEnumValues(Enums::UserType::Annonymous, Enums::UserType::UserTypeCount);
    return availableList;
}

QList<Enums::PlaceType> DataGlobals::availablePlaceTypes()
{
    static const auto availableList = Utility::generateEnumValues(Enums::PlaceType::Invalid, Enums::PlaceType::PlaceTypeCount);
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

QVariantList DataGlobals::userActionsQml(const Enums::UserType &user)
{
    return Utility::toVariantList(userActions(user), QMetaType::Int);
}

QVariantList DataGlobals::availableUserTypesQml()
{
    static auto variantList = Utility::toVariantList(availableUserTypes(), QMetaType::Int);
    return variantList;
}

QVariantList DataGlobals::availablePlaceTypesQml()
{
    static auto variantList = Utility::toVariantList(availablePlaceTypes(), QMetaType::Int);
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

DataGlobals::DataGlobals()
{}
