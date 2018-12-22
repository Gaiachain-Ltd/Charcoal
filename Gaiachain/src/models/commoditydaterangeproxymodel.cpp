#include "commoditydaterangeproxymodel.h"

#include "daterangeproxymodel.h"
#include "commodityproxymodel.h"
#include "eventmodel.h"
#include "shipmentmodel.h"

CommodityDateRangeProxyModel::CommodityDateRangeProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{

}

void CommodityDateRangeProxyModel::setCommodityProxyModel(CommodityProxyModel *commodityProxyModel)
{
    m_commodityProxyModel = commodityProxyModel;
}

void CommodityDateRangeProxyModel::setDateTimeRange(QDateTime start, QDateTime end)
{
    dynamic_cast<DateRangeProxyModel*>(sourceModel())->setDateTimeRange(start, end);
}

void CommodityDateRangeProxyModel::setCommodityType(Enums::CommodityType filterType)
{
    assert(m_commodityProxyModel != nullptr);

    m_commodityProxyModel->setCommodityType(filterType);
}

bool CommodityDateRangeProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent)

    auto shipmentId = sourceModel()->data(sourceModel()->index(sourceRow, 0), EventModel::ShipmentId).toInt();

    return commodityProxyModelContainsId(shipmentId);
}

bool CommodityDateRangeProxyModel::commodityProxyModelContainsId(int shipmentId) const
{
    assert(m_commodityProxyModel != nullptr);

    const int rowCount = m_commodityProxyModel->rowCount();

    for (int i = 0; i < rowCount; ++i) {
        if (m_commodityProxyModel->data(
                    m_commodityProxyModel->index(i, 0),ShipmentModel::ShipmentId).toInt() == shipmentId) {
            return true;
        }
    }

    return false;
}
