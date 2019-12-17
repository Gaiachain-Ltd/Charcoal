#ifndef COOPERATIVEIDQUERY_H
#define COOPERATIVEIDQUERY_H

#include "abstractsortfilterqueryextension.h"
#include "../eventmodel.h"

template <typename SFQ>
class CooperativeIdQuery final : public AbstractSortFilterQueryExtension<SFQ>
{
public:
    CooperativeIdQuery(quint32 cooperativeId, SFQ &&baseQuery)
        : AbstractSortFilterQueryExtension<SFQ>(baseQuery), m_cooperativeId(cooperativeId)
    {
        prepareSortFilter();
    }

    CooperativeIdQuery(quint32 cooperativeId, const SFQ &baseQuery)
        : AbstractSortFilterQueryExtension<SFQ>(baseQuery), m_cooperativeId(cooperativeId)
    {
        prepareSortFilter();
    }

protected:
    void prepareSortFilter() override
    {
        this->addFilter({ EventModel::columnName(EventModel::Columns::CooperativeId),
                          ISortFilterQuery::FilterType::Is, m_cooperativeId });
    }

private:
    quint32 m_cooperativeId = 0;
};

#endif // COOPERATIVEIDQUERY_H
