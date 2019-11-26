#include "packagedata.h"

#include "../helpers/utility.h"

QVariantList PackageData::actions() const
{
    return Utility::toVariantList(actionDates.keys(), QMetaType::Int);
}

QDateTime PackageData::actionDate(const Enums::SupplyChainAction &action) const
{
    return actionDates.value(action);
}
