#include "sortfilterquery.h"

#include "common/types.h"

namespace {
using OperatorTypeHandler = std::function<QPair<QString, QVariantList>(const SortFilterQuery::FilterQuery &)>;

QPair<QString, QVariantList> resolveSimpleFilter(const SortFilterQuery::FilterQuery &filterQuery);
QPair<QString, QVariantList> resolveMultipleFilter(const SortFilterQuery::FilterQuery &filterQuery);

const QMap<SortFilterQuery::FilterType, QLatin1String> OperatorTypeStr = {
    { SortFilterQuery::FilterType::Is, QLatin1String("IS") },
    { SortFilterQuery::FilterType::Not, QLatin1String("IS NOT") },
    { SortFilterQuery::FilterType::Like, QLatin1String("LIKE") },
    { SortFilterQuery::FilterType::Less, QLatin1String("<") },
    { SortFilterQuery::FilterType::LessEqual, QLatin1String("<=") },
    { SortFilterQuery::FilterType::Greater, QLatin1String(">") },
    { SortFilterQuery::FilterType::GreaterEqual, QLatin1String(">=") },
    { SortFilterQuery::FilterType::In, QLatin1String("IN") }
};
const QMap<SortFilterQuery::FilterType, OperatorTypeHandler> OperatorTypeHandlers = {
    { SortFilterQuery::FilterType::Is, &resolveSimpleFilter },
    { SortFilterQuery::FilterType::Not, &resolveSimpleFilter },
    { SortFilterQuery::FilterType::Like, &resolveSimpleFilter },
    { SortFilterQuery::FilterType::Less, &resolveSimpleFilter },
    { SortFilterQuery::FilterType::LessEqual, &resolveSimpleFilter },
    { SortFilterQuery::FilterType::Greater, &resolveSimpleFilter },
    { SortFilterQuery::FilterType::GreaterEqual, &resolveSimpleFilter },
    { SortFilterQuery::FilterType::In, &resolveMultipleFilter }
};

const QMap<Qt::SortOrder, QLatin1String> SortOrderStr = {
    { Qt::SortOrder::AscendingOrder, QLatin1String("ASC") },
    { Qt::SortOrder::DescendingOrder, QLatin1String("DESC") }
};

QPair<QString, QVariantList> resolveSimpleFilter(const ISortFilterQuery::FilterQuery &filterQuery)
{
    static const auto filterQueryTemplate = QString("`%1` %2 ?");

    return {
        filterQueryTemplate.arg(filterQuery.field).arg(OperatorTypeStr.value(filterQuery.op)),
        { types::convert(filterQuery.value, filterQuery.valueType), }
    };
}

QPair<QString, QVariantList> resolveMultipleFilter(const ISortFilterQuery::FilterQuery &filterQuery)
{
    static const auto filterQueryTemplate = QString("`%1` %2 (%3)");

    auto queryValuesString = QStringList{};
    auto queryBindValues = QVariantList{};
    for (const auto &value : filterQuery.value.toList()) {
        queryValuesString.append("?");
        queryBindValues.append(types::convert(value, filterQuery.valueType));
    }

    return {
        filterQueryTemplate.arg(filterQuery.field).arg(OperatorTypeStr.value(filterQuery.op)).arg(queryValuesString.join(", ")),
        queryBindValues
    };
}

}

SortFilterQuery::SortFilterQuery()
{}

SortFilterQuery::SortFilterQuery(const QList<SortFilterQuery::FilterQuery> &filterQueries, const QList<SortFilterQuery::SortQuery> &sortQueries)
    : m_filterQueries(filterQueries), m_sortQueries(sortQueries)
{}

QPair<QString, QVariantList> SortFilterQuery::resolve() const
{
    auto [ filtersQueryStr, bindValues ] = resolveFilters();
    return { filtersQueryStr + " " + resolveSorts(), bindValues };
}

QPair<QString, QVariantList> SortFilterQuery::resolveFilters() const
{
    if (m_filterQueries.isEmpty()) {
        return {};
    }

    static const auto joinOpStr = QString(" AND ");

    auto bindValues = QVariantList{};
    auto filtersQueryStr = QString{"WHERE "};
    for (const auto &filterQuery : m_filterQueries) {
        auto [ queryString, queryBindValues ] = OperatorTypeHandlers[filterQuery.op](filterQuery);
        filtersQueryStr.append(queryString + joinOpStr);
        bindValues.append(queryBindValues);
    }

    filtersQueryStr.remove(filtersQueryStr.size() - joinOpStr.size(), joinOpStr.size());
    return { filtersQueryStr, bindValues };
}

QString SortFilterQuery::resolveSorts() const
{
    if (m_sortQueries.isEmpty()) {
        return {};
    }

    static const auto joinOpStr = QString(", ");
    static const auto sortQueryTemplate = QString("`%1` %2" + joinOpStr);

    auto sortQueryStr = QString{"ORDER BY "};
    for (const auto &sortQuery : m_sortQueries) {
        sortQueryStr.append(sortQueryTemplate
                            .arg(sortQuery.field)
                            .arg(SortOrderStr.value(sortQuery.order))
                            );
    }

    if (!sortQueryStr.isEmpty()) {
        sortQueryStr.remove(sortQueryStr.size() - joinOpStr.size(), joinOpStr.size());
    }

    return sortQueryStr;
}

void SortFilterQuery::addFilter(const ISortFilterQuery::FilterQuery &query)
{
    m_filterQueries.append(query);
}

void SortFilterQuery::addSort(const ISortFilterQuery::SortQuery &query)
{
    m_sortQueries.append(query);
}
