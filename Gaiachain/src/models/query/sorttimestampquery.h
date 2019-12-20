#ifndef SORTTIMESTAMPQUERY_H
#define SORTTIMESTAMPQUERY_H

#include "abstractsortfilterqueryextension.h"
#include "../eventmodel.h"

template <typename SFQ>
class SortTimestampQuery final : public AbstractSortFilterQueryExtension<SFQ>
{
public:
    SortTimestampQuery(SFQ &&baseQuery)
        : AbstractSortFilterQueryExtension<SFQ>(baseQuery)
    {
        prepareSortFilter();
    }

    SortTimestampQuery(const SFQ &baseQuery)
        : AbstractSortFilterQueryExtension<SFQ>(baseQuery)
    {
        prepareSortFilter();
    }

protected:
    void prepareSortFilter() override
    {
        this->addSort({ EventModel::columnName(EventModel::Columns::Timestamp), Qt::DescendingOrder });
    }
};

#endif // SORTTIMESTAMPQUERY_H
