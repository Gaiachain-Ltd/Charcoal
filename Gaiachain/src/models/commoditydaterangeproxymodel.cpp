#include "commoditydaterangeproxymodel.h"

#include "commodityproxymodel.h"
#include "eventmodel.h"
#include "shipmentmodel.h"

#include "../common/logs.h"

CommodityDateRangeProxyModel::CommodityDateRangeProxyModel(QObject *parent)
    : AbstractSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);

    connect(this, &QAbstractItemModel::rowsInserted, this, &CommodityDateRangeProxyModel::onRowsInserted);
    connect(this, &QAbstractItemModel::rowsRemoved, this, &CommodityDateRangeProxyModel::onRowsRemoved);
    connect(this, &QAbstractItemModel::modelReset, this, &CommodityDateRangeProxyModel::onModelReset);
}

void CommodityDateRangeProxyModel::setCommodityProxyModel(CommodityProxyModel *commodityProxyModel)
{
    if (!m_commodityProxyModel && commodityProxyModel) {
        m_commodityProxyModel = commodityProxyModel;
        connect(m_commodityProxyModel, &CommodityProxyModel::modelChanged, this, &CommodityDateRangeProxyModel::invalidateFilter);
        invalidate();
    }
}

void CommodityDateRangeProxyModel::setSortingTypeAndRole(int role, int sortColumn, Qt::SortOrder order)
{
    if (role < Qt::UserRole || role >= EventModel::LastRole) {
        qWarning() << "Invlaid sort role" << role;
        return;
    }

    setSortRole(role);
    sort(sortColumn, order);
}

void CommodityDateRangeProxyModel::setDateTimeRange(const QDateTime &startDateTime, const QDateTime &endDateTime)
{
    if (m_startDateTime == startDateTime
            && m_endDateTime == endDateTime)
        return;

    qDebug() << "Start end dates" << startDateTime << endDateTime;
    m_startDateTime = startDateTime;
    m_endDateTime = endDateTime;

    invalidateFilter();
    emit dateRangeChanged();
}

bool CommodityDateRangeProxyModel::hasEvents(const QDate &date) const
{
    auto val = eventsCommodityTypes(date);
    return !eventsCommodityTypes(date).isEmpty();
}

QList<Enums::CommodityType> CommodityDateRangeProxyModel::eventsCommodityTypes(const QDate &date) const
{
    return m_dateEventsCommodityTypes.value(date).keys();
}

bool CommodityDateRangeProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    auto index = sourceModel()->index(sourceRow, 0, sourceParent);
    auto timestamp = sourceModel()->data(index, EventModel::Timestamp).toDateTime();

    if (isInDateTimeRange(timestamp) && m_commodityProxyModel) {
        auto shipmentId = sourceModel()->data(index, EventModel::ShipmentId).toString();
        return m_commodityProxyModel->hasShipment(shipmentId);
    }
    return false;
}

bool CommodityDateRangeProxyModel::isInDateTimeRange(QDateTime &dt) const
{
    return m_startDateTime <= dt && dt < m_endDateTime;
}

void CommodityDateRangeProxyModel::onRowsInserted(const QModelIndex &parent, int first, int last)
{
    auto changedDates = QSet<QDate>{};

    for (auto row = first; row <= last; ++row) {
        auto rowIndex = index(row, 0, parent);

        auto eventDate = data(rowIndex, EventModel::Timestamp).toDateTime().date();
        auto shipmentId = data(rowIndex, EventModel::ShipmentId).toString();

        auto commodityType = m_commodityProxyModel->shipmentCommodityType(shipmentId);

        auto &dateEventsCommodities = m_dateEventsCommodityTypes[eventDate];
        dateEventsCommodities[commodityType]++;
        if (dateEventsCommodities.value(commodityType) == 1)
            changedDates.insert(eventDate);
    }

    for (const auto &date : changedDates)
        emit eventsCommoditiesChanged(date);
}

void CommodityDateRangeProxyModel::onRowsRemoved(const QModelIndex &parent, int first, int last)
{
    auto changedDates = QSet<QDate>{};

    for (auto row = first; row <= last; ++row) {
        auto rowIndex = index(row, 0, parent);

        auto eventDate = data(rowIndex, EventModel::Timestamp).toDateTime().date();
        auto shipmentId = data(rowIndex, EventModel::ShipmentId).toString();

        auto commodityType = m_commodityProxyModel->shipmentCommodityType(shipmentId);

        auto &dateEventsCommodities = m_dateEventsCommodityTypes[eventDate];
        dateEventsCommodities[commodityType]--;
        if (dateEventsCommodities.value(commodityType) <= 0) {
            dateEventsCommodities.remove(commodityType);
            changedDates.insert(eventDate);
        }
    }

    for (const auto &date : changedDates)
        emit eventsCommoditiesChanged(date);
}

void CommodityDateRangeProxyModel::onModelReset()
{
    auto keys = m_dateEventsCommodityTypes.keys();
    m_dateEventsCommodityTypes.clear();

    while (!keys.isEmpty()) emit eventsCommoditiesChanged(keys.takeLast());
}

