#include "transactionsviewmodel.h"

#include "../query/cooperativeidquery.h"
#include "../query/sorttimestampquery.h"
#include "../query/packagetypequery.h"
#include "../query/idkeywordquery.h"

TransactionsViewModel::TransactionsViewModel(QObject *parent)
    : CooperativeViewModel(parent)
{

#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
        if (not DataGlobals::availablePackageTypes().isEmpty()) {
            for (auto it = DataGlobals::availablePackageTypes().begin(); it != DataGlobals::availablePackageTypes().end(); ++it) {
                m_activePackageTypes.insert(*it);
            }
        }
#else
        m_activePackageTypes= DataGlobals::availablePackageTypes().toSet()
#endif
}

void TransactionsViewModel::clear()
{
    auto activePackageTypes = m_activePackageTypes;

    m_keyword.clear();

#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    if (not DataGlobals::availablePackageTypes().isEmpty()) {
        m_activePackageTypes.clear();
        for (auto it = DataGlobals::availablePackageTypes().begin(); it != DataGlobals::availablePackageTypes().end(); ++it) {
            m_activePackageTypes.insert(*it);
        }
    }
#else
    m_activePackageTypes = QSet<Enums::PackageType>(
        DataGlobals::availablePackageTypes().toSet());
#endif

    m_cooperativeOnly = true;

    activePackageTypes = m_activePackageTypes - activePackageTypes;

    updateFilterQuery();
    emit keywordChanged(m_keyword);
    std::for_each(activePackageTypes.constBegin(), activePackageTypes.constEnd(),
                  std::bind(&TransactionsViewModel::packageTypeActiveChanged, this, std::placeholders::_1, true));
    emit cooperativeOnlyChanged(m_cooperativeOnly);
}

bool TransactionsViewModel::isPackageTypeActive(const Enums::PackageType &type) const
{
    return m_activePackageTypes.contains(type);
}

QString TransactionsViewModel::keyword() const
{
    return m_keyword;
}

QSet<Enums::PackageType> TransactionsViewModel::activePackageTypes() const
{
    return m_activePackageTypes;
}

void TransactionsViewModel::setPackageTypeActive(const Enums::PackageType &type, bool active)
{
    if (isPackageTypeActive(type) == active) {
        return;
    }

    if (active) {
        m_activePackageTypes.insert(type);
    } else {
        m_activePackageTypes.remove(type);
    }

    updateFilterQuery();
    emit packageTypeActiveChanged(type, active);
}

void TransactionsViewModel::setKeyword(const QString &keyword)
{
    if (m_keyword == keyword) {
        return;
    }

    m_keyword = keyword;
    updateFilterQuery();
    emit keywordChanged(m_keyword);
}

void TransactionsViewModel::updateFilterQuery()
{
    if (!m_queryModel.isNull()) {
        if (m_cooperativeOnly) {
            m_queryModel->setSortFilterQuery(IdKeywordQuery(m_keyword, false,
                                                            PackageTypeQuery(m_activePackageTypes,
                                                                             CooperativeIdQuery(m_cooperativeId,
                                                                                                SortTimestampQuery(SortFilterQuery()))))
                                             );
        } else {
            m_queryModel->setSortFilterQuery(IdKeywordQuery(m_keyword, false,
                                                            PackageTypeQuery(m_activePackageTypes,
                                                                             SortTimestampQuery(SortFilterQuery())))
                                             );
        }
    }
}
