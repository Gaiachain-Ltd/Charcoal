#ifndef LOCALEVENTSQUERY_H
#define LOCALEVENTSQUERY_H

#include "abstractsortfilterqueryextension.h"
#include "../eventmodel.h"

template <typename SFQ>
class LocalEventsQuery final : public AbstractSortFilterQueryExtension<SFQ>
{
public:
    LocalEventsQuery(SFQ &&baseQuery)
        : AbstractSortFilterQueryExtension<SFQ>(baseQuery)
    {
        prepareSortFilter();
    }

    LocalEventsQuery(const SFQ &baseQuery)
        : AbstractSortFilterQueryExtension<SFQ>(baseQuery)
    {
        prepareSortFilter();
    }

private:
    void prepareSortFilter() override
    {
        this->addFilter({ EventModel::columnName(EventModel::Columns::IsLocal),
                          ISortFilterQuery::FilterType::Is, true });
    }
};

#endif // LOCALEVENTSQUERY_H
