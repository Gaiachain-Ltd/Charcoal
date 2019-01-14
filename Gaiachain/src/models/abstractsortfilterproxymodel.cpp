#include "abstractsortfilterproxymodel.h"

AbstractSortFilterProxyModel::AbstractSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    connect(this, &AbstractSortFilterProxyModel::filteringStarted,
            this, &AbstractSortFilterProxyModel::proxyChangeStarted);
    connect(this, &AbstractSortFilterProxyModel::sortingFinished,
            this, &AbstractSortFilterProxyModel::proxyChangeFinished);
}

void AbstractSortFilterProxyModel::invalidateFilterNotify()
{
    emit filteringStarted();
    invalidateFilter();
    QMetaObject::invokeMethod(this, "filteringFinished");
}

void AbstractSortFilterProxyModel::invalidateSortNotify(int sortColumn, Qt::SortOrder order)
{
    emit sortingStarted();
    sort(sortColumn, order);
    QMetaObject::invokeMethod(this, "sortingFinished");
}

void AbstractSortFilterProxyModel::invalidateNotify(int sortColumn)
{
    invalidateFilterNotify();
    invalidateSortNotify(sortColumn);
}
