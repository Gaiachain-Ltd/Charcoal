#include "calendarviewmodel.h"

#include "../query/cooperativeidquery.h"
#include "../query/sorttimestampquery.h"
#include "../query/daterangequery.h"

CalendarViewModel::CalendarViewModel(QObject *parent)
    : CooperativeViewModel(parent)
{}

void CalendarViewModel::clear()
{
    m_fromDate = QDate{};
    m_toDate = QDate{};
    m_cooperativeOnly = true;

    updateFilterQuery();
    emit fromDateChanged(m_fromDate);
    emit toDateChanged(m_toDate);
    emit cooperativeOnlyChanged(m_cooperativeOnly);
}

QDate CalendarViewModel::fromDate() const
{
    return m_fromDate;
}

QDate CalendarViewModel::toDate() const
{
    return m_toDate;
}

QDateTime CalendarViewModel::fromDateTime() const
{
    static const auto StartDayTime = QTime(0, 0);
    return QDateTime(m_fromDate, StartDayTime);
}

QDateTime CalendarViewModel::toDateTime() const
{
    static const auto EndDayTime = QTime(23, 59, 59, 999);
    return QDateTime(m_toDate, EndDayTime);
}

void CalendarViewModel::setDateRange(const QDate &fromDate, const QDate &toDate)
{
    if (m_fromDate == fromDate &&
            m_toDate == toDate) {
        return;
    }

    m_fromDate = fromDate;
    m_toDate = toDate;

    updateFilterQuery();
    emit fromDateChanged(m_fromDate);
    emit toDateChanged(m_toDate);
}

void CalendarViewModel::setDate(const QDate &date)
{
    setDateRange(date, date);
}

void CalendarViewModel::setFromDate(const QDate &fromDate)
{
    if (m_fromDate == fromDate) {
        return;
    }

    m_fromDate = fromDate;
    updateFilterQuery();
    emit fromDateChanged(m_fromDate);
}

void CalendarViewModel::setToDate(const QDate &toDate)
{
    if (m_toDate == toDate) {
        return;
    }

    m_toDate = toDate;
    updateFilterQuery();
    emit toDateChanged(m_toDate);
}

void CalendarViewModel::updateFilterQuery()
{
    if (!m_queryModel.isNull()) {
        auto sortQuery = SortTimestampQuery(SortFilterQuery());
        if (m_cooperativeOnly) {
            updateFilterQuery(CooperativeIdQuery(m_cooperativeId, sortQuery));
        } else {
            updateFilterQuery(sortQuery);
        }
    }
}

template<typename BaseQuery>
void CalendarViewModel::updateFilterQuery(BaseQuery &&baseQuery)
{
    if (!m_queryModel.isNull()) {
        if (m_fromDate.isValid() && m_toDate.isValid()) {
            m_queryModel->setSortFilterQuery(DateRangeQuery(toDateTime(), DateRangeQueryBound::Type::To,
                                                            DateRangeQuery(fromDateTime(), DateRangeQueryBound::Type::From, baseQuery)));
        } else if (m_fromDate.isValid()) {
            m_queryModel->setSortFilterQuery(DateRangeQuery(fromDateTime(), DateRangeQueryBound::Type::From, baseQuery));
        } else if (m_toDate.isValid()) {
            m_queryModel->setSortFilterQuery(DateRangeQuery(toDateTime(), DateRangeQueryBound::Type::To, baseQuery));
        } else {
            m_queryModel->setSortFilterQuery(baseQuery);
        }
    }
}
