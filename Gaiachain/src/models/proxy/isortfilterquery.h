#ifndef ISORTFILTERQUERY_H
#define ISORTFILTERQUERY_H

#include <QVariant>

class ISortFilterQuery
{
public:
    enum class FilterType {
        Invalid = -1,
        Is,
        Not,
        Like,
        Less,
        LessEqual,
        Greater,
        GreaterEqual
    };

    struct FilterQuery {
        QString field;
        FilterType op = FilterType::Invalid;
        QVariant value;
        QMetaType::Type valueType = QMetaType::Void;
    };

    struct SortQuery {
        QString field;
        Qt::SortOrder order;
    };

    virtual ~ISortFilterQuery() = default;

    virtual QPair<QString, QVariantList> resolve() const = 0;
    virtual QPair<QString, QVariantList> resolveFilters() const = 0;
    virtual QString resolveSorts() const = 0;

    virtual void addFilter(const FilterQuery &query) = 0;
    virtual void addSort(const SortQuery &query) = 0;
};

#endif // ISORTFILTERQUERY_H
