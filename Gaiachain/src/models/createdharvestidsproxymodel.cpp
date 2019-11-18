#include "createdharvestidsproxymodel.h"

#include "eventmodel.h"
#include "../common/enums.h"
#include "../common/dataglobals.h"

CreatedHarvestIdsProxyModel::CreatedHarvestIdsProxyModel(QObject *parent)
    : AbstractSortFilterProxyModel(parent)
{}

bool CreatedHarvestIdsProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    auto index = sourceModel()->index(sourceRow, 0, sourceParent);

    auto action = sourceModel()->data(index, EventModel::Action).value<Enums::SupplyChainAction>();
    if (action == Enums::SupplyChainAction::Harvest) {
        auto packageId = sourceModel()->data(index, EventModel::PackageId).toString();

        auto actionsList = sourceModel()->match(sourceModel()->index(0, 0), EventModel::PackageId, packageId, -1, Qt::MatchExactly);
        Q_ASSERT(actionsList.size() >= 1);    // should at least found current action event
        return (actionsList.size() == 1);
    }
    return false;
}
