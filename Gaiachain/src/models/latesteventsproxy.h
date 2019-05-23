#ifndef LATESTEVENTSPROXY_H
#define LATESTEVENTSPROXY_H

#include "abstractsortfilterproxymodel.h"

class CommodityProxyModel;

class LatestEventsProxy : public AbstractSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit LatestEventsProxy(QObject *parent = nullptr);

    void setCommodityProxyModel(CommodityProxyModel *commodityProxyModel);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    CommodityProxyModel *m_commodityProxyModel;
};

#endif // LATESTEVENTSPROXY_H
