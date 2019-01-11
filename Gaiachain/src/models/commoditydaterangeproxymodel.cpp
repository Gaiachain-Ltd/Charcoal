#include "commoditydaterangeproxymodel.h"

#include "commodityproxymodel.h"
#include "eventmodel.h"
#include "shipmentmodel.h"

#include <QDebug>

CommodityDateRangeProxyModel::CommodityDateRangeProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{

}

void CommodityDateRangeProxyModel::setCommodityProxyModel(CommodityProxyModel *commodityProxyModel)
{
    m_commodityProxyModel = commodityProxyModel;
    connect(m_commodityProxyModel, &CommodityProxyModel::commodityTypeChanged, this, &CommodityDateRangeProxyModel::invalidateFilter);
}

void CommodityDateRangeProxyModel::setDateTimeRange(QDateTime start, QDateTime end)
{
    m_startDateTime = start;
    m_endDateTime = end;

    qDebug() << "Start end dates" << start << end;
    invalidateFilter();
}

bool CommodityDateRangeProxyModel::isEventToday(QDate date)
{
    Q_UNUSED(date)
//    for (int i = 0; i < rowCount(); ++i) {
//        auto index = sourceModel()->index(i, 0);
//        auto arrival = data(index, EventModel::ArrivalDateTime).toDate();
//        auto departure = data(index, EventModel::DepartureDateTime).toDate();

//        if (date == arrival || date == departure)
//            return true;
//    }

    return false;
}

bool CommodityDateRangeProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceRow)
    Q_UNUSED(sourceParent)
//    if (sourceRow < 0 || sourceRow > sourceModel()->rowCount())
//        return false;

//    auto index = sourceModel()->index(sourceRow, 0, sourceParent);

//    auto arrivalDateTime = sourceModel()->data(index, EventModel::ArrivalDateTime).toDateTime();
//    auto departureDateTime = sourceModel()->data(index, EventModel::DepartureDateTime).toDateTime();

//    bool containsId = false;
//    if (isInDateTimeRange(arrivalDateTime) || isInDateTimeRange(departureDateTime)) {
//        auto shipmentId = sourceModel()->data(index, EventModel::ShipmentId).toInt();
//        containsId = commodityProxyModelContainsId(shipmentId);
//    }

//    if (containsId) {
//        qDebug() << "Contains Arrival/Departure" << arrivalDateTime << departureDateTime;
//    }

//    return containsId;
    return true;
}

bool CommodityDateRangeProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    Q_UNUSED(source_left)
    Q_UNUSED(source_right)
    //TO_DO sort by date (arrival and departure)

    return true;
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
    return m_startDateTime <= dt && dt < m_endDateTime;
}

