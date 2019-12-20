#ifndef ABSTRACTSORTFILTERQUERYEXTENSION_H
#define ABSTRACTSORTFILTERQUERYEXTENSION_H

#include "isortfilterquery.h"
#include "sortfilterquery.h"

#include <type_traits>

template <typename SFQ, typename = std::enable_if_t<std::is_base_of<ISortFilterQuery, SFQ>::value>>
class AbstractSortFilterQueryExtension : public ISortFilterQuery
{
public:
    AbstractSortFilterQueryExtension(SFQ &&baseQuery)
        : m_baseQuery(std::move(baseQuery))
    {}

    AbstractSortFilterQueryExtension(const SFQ &baseQuery)
        : m_baseQuery(baseQuery)
    {}

    QPair<QString, QVariantList> resolve() const override
    {
        return m_baseQuery.resolve();
    }

    QPair<QString, QVariantList> resolveFilters() const override
    {
        return m_baseQuery.resolveFilters();
    }

    QString resolveSorts() const override
    {
        return m_baseQuery.resolveSorts();
    }

    void addFilter(const FilterQuery &query) override
    {
        m_baseQuery.addFilter(query);
    }

    void addSort(const SortQuery &query) override
    {
        m_baseQuery.addSort(query);
    }

protected:
    SFQ m_baseQuery;

    virtual void prepareSortFilter() = 0;
};

#endif // ABSTRACTSORTFILTERQUERYEXTENSION_H
