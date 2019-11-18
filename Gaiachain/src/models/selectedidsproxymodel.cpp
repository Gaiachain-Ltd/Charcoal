#include "selectedidsproxymodel.h"

#include "eventmodel.h"
#include "../common/enums.h"
#include "../common/dataglobals.h"

SelectedIdsProxyModel::SelectedIdsProxyModel(QObject *parent)
    : AbstractSortFilterProxyModel(parent)
{}

void SelectedIdsProxyModel::setPackageIds(const QStringList &packageIds)
{
    if (m_packageIds != packageIds) {
        m_packageIds = packageIds;
        invalidateFilter();
    }
}

bool SelectedIdsProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    auto index = sourceModel()->index(sourceRow, 0, sourceParent);
    auto packageId = sourceModel()->data(index, EventModel::PackageId).toString();
    if (!m_packageIds.contains(packageId)) {
        return false;
    }

    auto action = sourceModel()->data(index, EventModel::Action).value<Enums::SupplyChainAction>();
    auto packageType = DataGlobals::packageType(action);
    return (action == DataGlobals::packageTypeCreationAction(packageType));
}
