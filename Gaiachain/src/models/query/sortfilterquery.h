#ifndef SORTFILTERQUERY_H
#define SORTFILTERQUERY_H

#include "isortfilterquery.h"

#include <QVariant>

class SortFilterQuery final : public ISortFilterQuery
{
public:
    SortFilterQuery();
    SortFilterQuery(const QList<FilterQuery> &filterQueries, const QList<SortQuery> &sortQueries);

    QPair<QString, QVariantList> resolve() const override;
    QPair<QString, QVariantList> resolveFilters() const override;
    QString resolveSorts() const override;

    void addFilter(const FilterQuery &query) override;
    void addSort(const SortQuery &query) override;

private:
    QList<FilterQuery> m_filterQueries;
    QList<SortQuery> m_sortQueries;
};

#endif // SORTFILTERQUERY_H
