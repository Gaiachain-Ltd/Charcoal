#include "daterangeproxymodel.h"

#include "eventmodel.h"

#include "../common/logs.h"

DateRangeProxyModel::DateRangeProxyModel(QObject *parent)
    : AbstractSortFilterProxyModel(parent)
{
}

void DateRangeProxyModel::setSingleDateRange(const QDate &date)
{
    setDateRange(date, date);
}

void DateRangeProxyModel::setDateRange(const QDate &startDate, const QDate &endDate)
{
    static const auto dayStart = QTime(0, 0);
    static const auto dayEnd = QTime(23, 59, 59, 999);
    setDateTimeRange(QDateTime(startDate, dayStart), QDateTime(endDate, dayEnd));
}

void DateRangeProxyModel::setDateTimeRange(const QDateTime &startDateTime, const QDateTime &endDateTime)
{
    if (m_startDateTime == startDateTime
            && m_endDateTime == endDateTime) {
        return;
    }
    if (startDateTime > endDateTime) {
        qCWarning(dataModels) << "Start date is bigger than end date!";
    }

    qCDebug(dataModels) << "Start end dates:" << startDateTime << endDateTime;
    m_startDateTime = startDateTime;
    m_endDateTime = endDateTime;

    resetFilter();
    emit dateRangeChanged(startDateTime, endDateTime);
}

QDateTime DateRangeProxyModel::startDateTime() const
{
    return m_startDateTime;
}

QDateTime DateRangeProxyModel::endDateTime() const
{
    return m_endDateTime;
}

bool DateRangeProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    auto index = sourceModel()->index(sourceRow, 0, sourceParent);
    auto timestamp = sourceModel()->data(index, EventModel::Timestamp).toDateTime();

    return isInDateTimeRange(timestamp);
}

bool DateRangeProxyModel::isInDateTimeRange(const QDateTime &dt) const
{
    return m_startDateTime <= dt && dt <= m_endDateTime;
}
