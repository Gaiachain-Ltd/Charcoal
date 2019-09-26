#ifndef LATESTEVENTSPROXY_H
#define LATESTEVENTSPROXY_H

#include "abstractsortfilterproxymodel.h"

class LatestEventsProxy : public AbstractSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit LatestEventsProxy(QObject *parent = nullptr);
};

#endif // LATESTEVENTSPROXY_H
