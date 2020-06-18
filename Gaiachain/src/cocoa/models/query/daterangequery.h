#ifndef DATERANGEQUERY_H
#define DATERANGEQUERY_H

#include "abstractsortfilterqueryextension.h"
#include "../eventmodel.h"

#include <QDate>

namespace DateRangeQueryBound {
enum class Type {
    From,
    To,
    Equal
};
}

template <typename SFQ>
class DateRangeQuery final : public AbstractSortFilterQueryExtension<SFQ>
{
public:
    DateRangeQuery(const QDateTime &dateTime, const DateRangeQueryBound::Type &type, SFQ &&baseQuery)
        : AbstractSortFilterQueryExtension<SFQ>(baseQuery), m_dateTime(dateTime), m_type(type)
    {
        prepareSortFilter();
    }

    DateRangeQuery(const QDateTime &dateTime, const DateRangeQueryBound::Type &type, const SFQ &baseQuery)
        : AbstractSortFilterQueryExtension<SFQ>(baseQuery), m_dateTime(dateTime), m_type(type)
    {
        prepareSortFilter();
    }

protected:
    void prepareSortFilter() override
    {
        this->addFilter({ EventModel::columnName(EventModel::Columns::Timestamp),
                          sc_boundFilterType.value(m_type),
                          m_dateTime, QMetaType::LongLong });
    }

private:
    static const QMap<DateRangeQueryBound::Type, ISortFilterQuery::FilterType> sc_boundFilterType;

    QDateTime m_dateTime;
    DateRangeQueryBound::Type m_type;
};

template <typename SFQ>
const QMap<DateRangeQueryBound::Type, ISortFilterQuery::FilterType> DateRangeQuery<SFQ>::sc_boundFilterType = {
    { DateRangeQueryBound::Type::From, ISortFilterQuery::FilterType::GreaterEqual },
    { DateRangeQueryBound::Type::To, ISortFilterQuery::FilterType::LessEqual },
    { DateRangeQueryBound::Type::Equal, ISortFilterQuery::FilterType::Is },
};

#endif // DATERANGEQUERY_H
