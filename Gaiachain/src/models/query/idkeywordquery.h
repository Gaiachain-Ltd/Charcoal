#ifndef IDKEYWORDQUERY_H
#define IDKEYWORDQUERY_H

#include "abstractsortfilterqueryextension.h"
#include "../eventmodel.h"

template <typename SFQ>
class IdKeywordQuery final : public AbstractSortFilterQueryExtension<SFQ>
{
public:
    IdKeywordQuery(const QString &keyword, bool exact, SFQ &&baseQuery)
        : AbstractSortFilterQueryExtension<SFQ>(baseQuery), m_keyword(keyword), m_exact(exact)
    {
        prepareSortFilter();
    }

    IdKeywordQuery(const QString &keyword, bool exact, const SFQ &baseQuery)
        : AbstractSortFilterQueryExtension<SFQ>(baseQuery), m_keyword(keyword), m_exact(exact)
    {
        prepareSortFilter();
    }

protected:
    void prepareSortFilter() override
    {
        static const auto containsKeywordStr = QString{"%%1%"};
        if (m_exact) {
            this->addFilter({ EventModel::columnName(EventModel::Columns::PackageId),
                              ISortFilterQuery::FilterType::Is, m_keyword });
        } else {
            this->addFilter({ EventModel::columnName(EventModel::Columns::PackageId),
                              ISortFilterQuery::FilterType::Like, containsKeywordStr.arg(m_keyword) });
        }
    }

private:
    QString m_keyword;
    bool m_exact = false;
};

#endif // IDKEYWORDQUERY_H
