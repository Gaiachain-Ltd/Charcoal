#include "latesteventsproxy.h"

#include "eventmodel.h"

#include <QDebug>

LatestEventsProxy::LatestEventsProxy(QObject *parent)
    : AbstractSortFilterProxyModel (parent)
{
    setDynamicSortFilter(true);

    setSortRole(EventModel::Timestamp);
    sort(0, Qt::SortOrder::DescendingOrder);
}

bool LatestEventsProxy::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    return sourceModel()->index(sourceRow, 0, sourceParent).isValid();
}
