#include "daterangeproxymodel.h"

#include "eventmodel.h"

#include "../common/logs.h"

DateRangeProxyModel::DateRangeProxyModel(QObject *parent)
    : AbstractSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);

    connect(this, &QAbstractItemModel::rowsInserted, this, &DateRangeProxyModel::onRowsInserted);
    connect(this, &QAbstractItemModel::rowsRemoved, this, &DateRangeProxyModel::onRowsRemoved);
    connect(this, &QAbstractItemModel::modelReset, this, &DateRangeProxyModel::onModelReset);
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

    invalidateFilter();
    emit dateRangeChanged();
}

void DateRangeProxyModel::setSortingTypeAndRole(int role, int sortColumn, Qt::SortOrder order)
{
    if (role < Qt::UserRole || role >= EventModel::LastRole) {
        qCWarning(dataModels) << "Invlaid sort role" << role;
        return;
    }

    setSortRole(role);
    sort(sortColumn, order);
}

bool DateRangeProxyModel::hasEvents(const QDate &date) const
{
    return m_dateEvents.contains(date);
}

bool DateRangeProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    auto index = sourceModel()->index(sourceRow, 0, sourceParent);
    auto timestamp = sourceModel()->data(index, EventModel::Timestamp).toDateTime();

    return isInDateTimeRange(timestamp);
}

bool DateRangeProxyModel::isInDateTimeRange(const QDateTime &dt) const
{
    return m_startDateTime <= dt && dt < m_endDateTime;
}

void DateRangeProxyModel::onRowsInserted(const QModelIndex &parent, int first, int last)
{
    for (auto row = first; row <= last; ++row) {
        auto rowIndex = index(row, 0, parent);

        auto eventDate = data(rowIndex, EventModel::Timestamp).toDateTime().date();
        m_dateEvents.insert(eventDate, m_dateEvents.value(eventDate, 0) + 1);

        if (m_dateEvents.value(eventDate) == 1) {   // first event on the date
            emit hasEventsChanged(eventDate, true);
        }
    }
}

void DateRangeProxyModel::onRowsRemoved(const QModelIndex &parent, int first, int last)
{
    for (auto row = first; row <= last; ++row) {
        auto rowIndex = index(row, 0, parent);

        auto eventDate = data(rowIndex, EventModel::Timestamp).toDateTime().date();
        m_dateEvents.insert(eventDate, m_dateEvents.value(eventDate, 1) - 1);   // avoid having negative

        if (m_dateEvents.value(eventDate) == 0) {   // last event on the date
            emit hasEventsChanged(eventDate, false);
        }
    }
}

void DateRangeProxyModel::onModelReset()
{
    const auto dates = m_dateEvents.keys();
    m_dateEvents.clear();

    for (const auto &date : dates) {
        emit hasEventsChanged(date, false);
    }
}

