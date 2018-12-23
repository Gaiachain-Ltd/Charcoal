#include "commodityproxymodel.h"

#include "shipmentmodel.h"

CommodityProxyModel::CommodityProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{

}

void CommodityProxyModel::setCommodityType(Enums::CommodityType filterType, bool enable)
{
    if (enable) {
        m_enabledCommodites.insert(filterType);
    } else {
        m_enabledCommodites.remove(filterType);
    }
}

bool CommodityProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent)

    auto commodityType = sourceModel()->data(sourceModel()->index(sourceRow, 0), ShipmentModel::Commodity).value<Enums::CommodityType>();

    return m_enabledCommodites.contains(commodityType);
}
