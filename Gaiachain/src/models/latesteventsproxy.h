#ifndef LATESTEVENTSPROXY_H
#define LATESTEVENTSPROXY_H

#include "abstractsortfilterproxymodel.h"

class LatestEventsProxy : public AbstractSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit LatestEventsProxy(QObject *parent = nullptr);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
};

#endif // LATESTEVENTSPROXY_H
