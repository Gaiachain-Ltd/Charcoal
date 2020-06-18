#ifndef COMPANYTYPEQUERY_H
#define COMPANYTYPEQUERY_H

#include "abstractsortfilterqueryextension.h"
#include "../companymodel.h"
#include "common/enums.h"

template <typename SFQ>
class CompanyTypeQuery final : public AbstractSortFilterQueryExtension<SFQ>
{
public:
    CompanyTypeQuery(const Enums::CompanyType &companyType, SFQ &&baseQuery)
        : AbstractSortFilterQueryExtension<SFQ>(baseQuery), m_companyType(companyType)
    {
        prepareSortFilter();
    }

    CompanyTypeQuery(const Enums::CompanyType &companyType, const SFQ &baseQuery)
        : AbstractSortFilterQueryExtension<SFQ>(baseQuery), m_companyType(companyType)
    {
        prepareSortFilter();
    }

protected:
    void prepareSortFilter() override
    {
        this->addFilter({ CompanyModel::columnName(CompanyModel::Columns::Type), ISortFilterQuery::FilterType::Is,
                          QVariant::fromValue(m_companyType), QMetaType::UInt });
    }

private:
    Enums::CompanyType m_companyType = Enums::CompanyType::Unknown;
};

#endif // COMPANYTYPEQUERY_H
