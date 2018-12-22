#include "daterangeproxymodel.h"

#include "eventmodel.h"

DateRangeProxyModel::DateRangeProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{

}

void DateRangeProxyModel::setDateTimeRange(QDateTime start, QDateTime end)
{
    m_startDateTime = start;
    m_endDateTime = end;
}

bool DateRangeProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent)

    auto index = sourceModel()->index(sourceRow, 0);

    auto arrivalDateTime = sourceModel()->data(index, EventModel::ArrivalDateTime).toDateTime();
    auto departureDateTime = sourceModel()->data(index, EventModel::DepartureDateTime).toDateTime();

    return isInDateTimeRange(arrivalDateTime) || isInDateTimeRange(departureDateTime);
}

bool DateRangeProxyModel::isInDateTimeRange(QDateTime &dt) const
{
    return m_startDateTime < dt && dt < m_endDateTime;
}
