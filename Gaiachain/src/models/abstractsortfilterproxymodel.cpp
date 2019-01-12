#include "abstractsortfilterproxymodel.h"

AbstractSortFilterProxyModel::AbstractSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{}

void AbstractSortFilterProxyModel::invalidateFilterNotify()
{
    emit filteringStarted();
    invalidateFilter();
    QMetaObject::invokeMethod(this, "filteringFinished");
}
