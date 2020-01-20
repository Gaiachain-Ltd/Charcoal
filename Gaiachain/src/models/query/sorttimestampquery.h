#ifndef SORTTIMESTAMPQUERY_H
#define SORTTIMESTAMPQUERY_H

#include "abstractsortfilterqueryextension.h"
#include "../eventmodel.h"

template <typename SFQ>
class SortTimestampQuery final : public AbstractSortFilterQueryExtension<SFQ>
{
public:
    SortTimestampQuery(SFQ &&baseQuery, Qt::SortOrder order = Qt::DescendingOrder)
        : AbstractSortFilterQueryExtension<SFQ>(baseQuery), m_order(order)
    {
        prepareSortFilter();
    }

    SortTimestampQuery(const SFQ &baseQuery, Qt::SortOrder order = Qt::DescendingOrder)
        : AbstractSortFilterQueryExtension<SFQ>(baseQuery), m_order(order)
    {
        prepareSortFilter();
    }

protected:
    Qt::SortOrder m_order = Qt::DescendingOrder;

    void prepareSortFilter() override
    {
        this->addSort({ EventModel::columnName(EventModel::Columns::Timestamp), m_order });
    }
};

#endif // SORTTIMESTAMPQUERY_H
