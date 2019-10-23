#include "searcheventsproxymodel.h"

#include "eventmodel.h"

SearchEventsProxyModel::SearchEventsProxyModel(QObject *parent)
    : AbstractSortFilterProxyModel(parent)
{
    setFilterRole(EventModel::PackageId);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
}

void SearchEventsProxyModel::search(const QString &keyWord, bool exact)
{
    if (exact) {
        setFilterFixedString(keyWord);
    } else {
        setFilterWildcard('*' + keyWord + '*');
    }
}
