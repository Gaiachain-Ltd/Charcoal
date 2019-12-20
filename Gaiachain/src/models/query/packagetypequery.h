#ifndef PACKAGETYPEQUERY_H
#define PACKAGETYPEQUERY_H

#include "abstractsortfilterqueryextension.h"

#include "../eventmodel.h"
#include "../../common/enums.h"
#include "../../common/dataglobals.h"
#include "../../helpers/utility.h"

template <typename SFQ>
class PackageTypeQuery final : public AbstractSortFilterQueryExtension<SFQ>
{
public:
    PackageTypeQuery(const QSet<Enums::PackageType> &packageTypes, SFQ &&baseQuery)
        : AbstractSortFilterQueryExtension<SFQ>(baseQuery), m_packageTypes(packageTypes)
    {
        prepareSortFilter();
    }

    PackageTypeQuery(const QSet<Enums::PackageType> &packageTypes, const SFQ &baseQuery)
        : AbstractSortFilterQueryExtension<SFQ>(baseQuery), m_packageTypes(packageTypes)
    {
        prepareSortFilter();
    }

protected:
    void prepareSortFilter() override
    {
        auto actionsList = QVariantList{};
        std::for_each(m_packageTypes.constBegin(), m_packageTypes.constEnd(),
                      [&actionsList] (const auto &packageType) {
            actionsList.append(Utility::toVariantList(DataGlobals::packageActions(packageType)));
        });

        this->addFilter({ EventModel::columnName(EventModel::Columns::Action),
                          ISortFilterQuery::FilterType::In, actionsList, QMetaType::UInt });
    }

private:
    QSet<Enums::PackageType> m_packageTypes;
};

#endif // PACKAGETYPEQUERY_H
