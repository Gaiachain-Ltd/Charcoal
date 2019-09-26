#include "latesteventsproxy.h"

#include "eventmodel.h"

#include "../common/logs.h"

LatestEventsProxy::LatestEventsProxy(QObject *parent)
    : AbstractSortFilterProxyModel (parent)
{
    setDynamicSortFilter(true);

    setSortRole(EventModel::Timestamp);
    sort(0, Qt::SortOrder::DescendingOrder);
}
