#include "searcheventsproxymodel.h"

#include "eventmodel.h"

SearchEventsProxyModel::SearchEventsProxyModel(QObject *parent)
    : AbstractSortFilterProxyModel(parent)
{
    setFilterRole(EventModel::PackageId);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
}

void SearchEventsProxyModel::search(const QString &keyword)
{
    if (m_keyword != keyword) {
        m_keyword = keyword;
        setFilterWildcard('*' + keyword + '*');

        emit keywordChanged(keyword);
    }
}

void SearchEventsProxyModel::clearSearch()
{
    if (!m_keyword.isNull()) {
        m_keyword = QString{};
        setFilterRegExp(QRegExp{});

        emit keywordChanged(m_keyword);
    }
}

QString SearchEventsProxyModel::keyword() const
{
    return m_keyword;
}
