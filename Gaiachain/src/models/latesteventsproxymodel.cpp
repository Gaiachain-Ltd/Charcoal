#include "latesteventsproxymodel.h"

#include "eventmodel.h"

#include "../common/logs.h"

LatestEventsProxyModel::LatestEventsProxyModel(QObject *parent)
    : AbstractSortFilterProxyModel(parent)
{
    setSortRole(EventModel::Timestamp);
    sort(0, Qt::SortOrder::DescendingOrder);
}
