#include "commodityproxymodel.h"

#include "shipmentmodel.h"

#include <QTimer>
#include <QDebug>

CommodityProxyModel::CommodityProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
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

    invalidateFilter();
    emit commodityTypeChanged();
}

bool CommodityProxyModel::commodityEnabled(Enums::CommodityType filterType) const
{
    return m_enabledCommodites.contains(filterType);
}

bool CommodityProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    auto srcIdx = sourceModel()->index(sourceRow, 0, sourceParent);
    if (!srcIdx.isValid()) {
        QTimer::singleShot(250, this, &CommodityProxyModel::filteringFinished);
        return false;
    }

    auto commodityType = sourceModel()->data(srcIdx, ShipmentModel::Commodity).value<Enums::CommodityType>();

    return commodityEnabled(commodityType);
}
