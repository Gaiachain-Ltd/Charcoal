#include "packagetypeproxymodel.h"

#include "eventmodel.h"
#include "../common/dataglobals.h"

PackageTypeProxyModel::PackageTypeProxyModel(QObject *parent)
    : AbstractSortFilterProxyModel(parent)
{
}

void PackageTypeProxyModel::setPackageTypeFiltering(const Enums::PackageType &type, bool active)
{
    if (isActive(type) == active) {
        return;
    }

    m_packageTypesActivity.insert(type, active);
    invalidateFilter();
}

void PackageTypeProxyModel::clearFiltering()
{
    auto filteringValues = m_packageTypesActivity.values().toSet();
    if (filteringValues.size() > 1 ||
            (!filteringValues.isEmpty() && filteringValues.contains(true))) {
        m_packageTypesActivity = {};
        invalidateFilter();
    }
}

bool PackageTypeProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    auto index = sourceModel()->index(sourceRow, 0, sourceParent);
    auto action = sourceModel()->data(index, EventModel::Action).value<Enums::SupplyChainAction>();
    auto packageType = DataGlobals::packageType(action);

    return isActive(packageType);
}

bool PackageTypeProxyModel::isActive(const Enums::PackageType &type) const
{
    return m_packageTypesActivity.value(type, true);
}
