#include "commoditydaterangeproxymodel.h"

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
    connect(m_commodityProxyModel, &CommodityProxyModel::commodityTypeChanged, this, &CommodityDateRangeProxyModel::invalidateFilter);
}

#include <QDebug>

void CommodityDateRangeProxyModel::setDateTimeRange(QDateTime start, QDateTime end)
{
    m_startDateTime = start;
    m_endDateTime = end;

    qDebug() << "Start end dates" << start << end;
    invalidateFilter();
}

bool CommodityDateRangeProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (sourceRow < 0 || sourceRow > sourceModel()->rowCount())
        return false;

    auto index = sourceModel()->index(sourceRow, 0, sourceParent);

    auto arrivalDateTime = sourceModel()->data(index, EventModel::ArrivalDateTime).toDateTime();
    auto departureDateTime = sourceModel()->data(index, EventModel::DepartureDateTime).toDateTime();

    //qDebug() << "Arrival/Departure" << arrivalDateTime << departureDateTime;

    if (isInDateTimeRange(arrivalDateTime) || isInDateTimeRange(departureDateTime)) {
        auto shipmentId = sourceModel()->data(index, EventModel::ShipmentId).toInt();
        return commodityProxyModelContainsId(shipmentId);
    }

    return false;
}

bool CommodityDateRangeProxyModel::commodityProxyModelContainsId(int shipmentId) const
{
    Q_ASSERT(m_commodityProxyModel != nullptr);

    const int rowCount = m_commodityProxyModel->rowCount();

    for (int i = 0; i < rowCount; ++i) {
        if (m_commodityProxyModel->data(
                    m_commodityProxyModel->index(i, 0),ShipmentModel::ShipmentId).toInt() == shipmentId) {
            return true;
        }
    }

    return false;
}

bool CommodityDateRangeProxyModel::isInDateTimeRange(QDateTime &dt) const
{
    return m_startDateTime <= dt && dt <= m_endDateTime;
}

