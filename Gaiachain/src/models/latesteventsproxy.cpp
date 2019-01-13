#include "latesteventsproxy.h"

#include "eventmodel.h"

LatestEventsProxy::LatestEventsProxy(QObject *parent)
    : AbstractSortFilterProxyModel (parent)
{
    setDynamicSortFilter(true);
    setSortRole(EventModel::Timestamp);
}

bool LatestEventsProxy::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceRow)
    Q_UNUSED(sourceParent)

    return true;
}
