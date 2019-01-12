#include "abstractsortfilterproxymodel.h"

AbstractSortFilterProxyModel::AbstractSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{}

void AbstractSortFilterProxyModel::invalidateFilterNotify()
{
    invalidateFilter();
    QMetaObject::invokeMethod(this, "filteringFinished");
}
