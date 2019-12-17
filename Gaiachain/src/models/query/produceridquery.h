#ifndef PRODUCERIDQUERY_H
#define PRODUCERIDQUERY_H

#include "abstractsortfilterqueryextension.h"
#include "../parcelmodel.h"

template <typename SFQ>
class ProducerIdQuery final : public AbstractSortFilterQueryExtension<SFQ>
{
public:
    ProducerIdQuery(quint32 producerId, SFQ &&baseQuery)
        : AbstractSortFilterQueryExtension<SFQ>(baseQuery), m_producerId(producerId)
    {
        prepareSortFilter();
    }

    ProducerIdQuery(quint32 producerId, const SFQ &baseQuery)
        : AbstractSortFilterQueryExtension<SFQ>(baseQuery), m_producerId(producerId)
    {
        prepareSortFilter();
    }

protected:
    void prepareSortFilter() override
    {
        this->addFilter({ ParcelModel::columnName(ParcelModel::Columns::ProducerId),
                          ISortFilterQuery::FilterType::Is, m_producerId });
    }

private:
    quint32 m_producerId = 0;
};

#endif // PRODUCERIDQUERY_H
