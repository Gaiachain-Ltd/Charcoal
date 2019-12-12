#include "latesteventsproxymodel.h"

#include "eventmodel.h"

LatestEventsProxyModel::LatestEventsProxyModel(QObject *parent)
    : AbstractSortFilterProxyModel(parent)
{
    setSortRole(EventModel::Timestamp);
    sort(0, Qt::SortOrder::DescendingOrder);
}
