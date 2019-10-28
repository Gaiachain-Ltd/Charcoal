#include "packagetypeidsproxymodel.h"

#include "eventmodel.h"
#include "../common/dataglobals.h"

PackageTypeIdsProxyModel::PackageTypeIdsProxyModel(QObject *parent)
    : AbstractSortFilterProxyModel(parent)
{
}

void PackageTypeIdsProxyModel::setPackageType(const Enums::PackageType &type)
{
    if (m_packageType == type) {
        return;
    }

    m_packageType = type;
    m_creationAction = DataGlobals::packageTypeCreationAction(type);
    invalidateFilter();
}

bool PackageTypeIdsProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    auto index = sourceModel()->index(sourceRow, 0, sourceParent);
    auto action = sourceModel()->data(index, EventModel::Action).value<Enums::SupplyChainAction>();
    return (action == m_creationAction);
}
