#include "latesteventsproxy.h"

#include "eventmodel.h"
#include "commodityproxymodel.h"

#include "../common/logs.h"

LatestEventsProxy::LatestEventsProxy(QObject *parent)
    : AbstractSortFilterProxyModel (parent)
{
    setDynamicSortFilter(true);

    setSortRole(EventModel::Timestamp);
    sort(0, Qt::SortOrder::DescendingOrder);
}

bool LatestEventsProxy::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    auto index = sourceModel()->index(sourceRow, 0, sourceParent);
    if (!index.isValid() || !m_commodityProxyModel)
        return false;

    auto shipmentId = sourceModel()->data(index, EventModel::ShipmentId).toString();
    return m_commodityProxyModel->hasShipment(shipmentId);
}

void LatestEventsProxy::setCommodityProxyModel(CommodityProxyModel *commodityProxyModel)
{
    if (!m_commodityProxyModel && commodityProxyModel) {
        m_commodityProxyModel = commodityProxyModel;
        connect(m_commodityProxyModel, &CommodityProxyModel::modelChanged, this, &LatestEventsProxy::invalidate);
        invalidate();
    }
}
