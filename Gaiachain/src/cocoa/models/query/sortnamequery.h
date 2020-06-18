#ifndef SORTNAMEQUERY_H
#define SORTNAMEQUERY_H

#include "abstractsortfilterqueryextension.h"

template <typename SFQ>
class SortNameQuery final : public AbstractSortFilterQueryExtension<SFQ>
{
public:
    SortNameQuery(SFQ &&baseQuery)
        : AbstractSortFilterQueryExtension<SFQ>(baseQuery)
    {
        prepareSortFilter();
    }

    SortNameQuery(const SFQ &baseQuery)
        : AbstractSortFilterQueryExtension<SFQ>(baseQuery)
    {
        prepareSortFilter();
    }

protected:
    void prepareSortFilter() override
    {
        this->addSort({ sc_nameField, Qt::AscendingOrder });
    }

private:
    static const QLatin1String sc_nameField;
};

template <typename SFQ>
const QLatin1String SortNameQuery<SFQ>::sc_nameField = QLatin1String("name");

#endif // SORTNAMEQUERY_H
