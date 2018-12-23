#include "commodityproxymodel.h"

#include "shipmentmodel.h"

#include <QDebug>

CommodityProxyModel::CommodityProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{

}

void CommodityProxyModel::setCommodityType(Enums::CommodityType filterType, bool enable)
{
    qDebug() << "FilterType" << filterType << enable;

    if (enable) {
        m_enabledCommodites.insert(filterType);
    } else {
        m_enabledCommodites.remove(filterType);
    }

    invalidateFilter();
    emit commodityTypeChanged();
}

bool CommodityProxyModel::commodityEnabled(Enums::CommodityType filterType) const
{
    return m_enabledCommodites.contains(filterType);
}

bool CommodityProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (sourceRow < 0 || sourceRow > sourceModel()->rowCount())
        return false;

    auto commodityType = sourceModel()->data(sourceModel()->index(sourceRow, 0, sourceParent), ShipmentModel::Commodity).value<Enums::CommodityType>();

    return commodityEnabled(commodityType);
}
