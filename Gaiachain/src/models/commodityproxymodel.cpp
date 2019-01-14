#include "commodityproxymodel.h"

#include "shipmentmodel.h"

#include <QTimer>
#include <QDebug>

CommodityProxyModel::CommodityProxyModel(QObject *parent)
    : AbstractSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);

    connect(this, &QAbstractItemModel::rowsInserted, this, &CommodityProxyModel::onRowsInserted, Qt::QueuedConnection);
    connect(this, &QAbstractItemModel::rowsRemoved, this, &CommodityProxyModel::onRowsRemoved, Qt::QueuedConnection);
    connect(this, &QAbstractItemModel::modelReset, this, &CommodityProxyModel::onModelReset, Qt::QueuedConnection);
}

void CommodityProxyModel::setCommodityType(Enums::CommodityType filterType, bool enable)
{
    if (m_enabledCommodites.contains(filterType) == enable)
        return;

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

bool CommodityProxyModel::hasShipment(const QString &shipmentId) const
{
    return m_shipmentsType.contains(shipmentId);
}

Enums::CommodityType CommodityProxyModel::shipmentCommodityType(const QString &shipmentId) const
{
    return m_shipmentsType.value(shipmentId);
}

bool CommodityProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    auto index = sourceModel()->index(sourceRow, 0, sourceParent);
    auto commodityType = sourceModel()->data(index, ShipmentModel::Commodity).value<Enums::CommodityType>();
    return commodityEnabled(commodityType);
}

void CommodityProxyModel::onRowsInserted(const QModelIndex &parent, int first, int last)
{
    for (auto row = first; row <= last; ++row) {
        auto rowIndex = index(row, 0, parent);

        auto shipmentId = data(rowIndex, ShipmentModel::ShipmentId).toString();
        auto commodityType = data(rowIndex, ShipmentModel::Commodity).value<Enums::CommodityType>();

        m_shipmentsType.insert(shipmentId, commodityType);
    }
}

void CommodityProxyModel::onRowsRemoved(const QModelIndex &parent, int first, int last)
{
    for (auto row = first; row <= last; ++row) {
        auto rowIndex = index(row, 0, parent);

        auto shipmentId = data(rowIndex, ShipmentModel::ShipmentId).toString();
        m_shipmentsType.remove(shipmentId);
    }
}

void CommodityProxyModel::onModelReset()
{
    m_shipmentsType.clear();
}
