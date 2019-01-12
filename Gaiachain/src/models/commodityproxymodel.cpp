#include "commodityproxymodel.h"

#include "shipmentmodel.h"

#include <QTimer>
#include <QDebug>

CommodityProxyModel::CommodityProxyModel(QObject *parent)
    : AbstractSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
}

void CommodityProxyModel::setCommodityType(Enums::CommodityType filterType, bool enable)
{
    qDebug() << "FilterType" << filterType << enable;

    if (enable) {
        m_enabledCommodites.insert(filterType);
    } else {
        m_enabledCommodites.remove(filterType);
    }

    m_shipmentIds.clear();
    invalidateFilterNotify();
    emit commodityTypeChanged();
}

bool CommodityProxyModel::commodityEnabled(Enums::CommodityType filterType) const
{
    return m_enabledCommodites.contains(filterType);
}

bool CommodityProxyModel::hasShipmentId(const QString &id) const
{
    return m_shipmentIds.contains(id);
}

bool CommodityProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    auto index = sourceModel()->index(sourceRow, 0, sourceParent);
    auto commodityType = sourceModel()->data(index, ShipmentModel::Commodity).value<Enums::CommodityType>();
    if (commodityEnabled(commodityType)) {
        m_shipmentIds.insert(sourceModel()->data(index, ShipmentModel::ShipmentId).toString());
        return true;
    }

    return false;
}
