#include "abstractsortfilterproxymodel.h"

AbstractSortFilterProxyModel::AbstractSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    connect(this, &AbstractSortFilterProxyModel::rowsInserted,
            this, &AbstractSortFilterProxyModel::modelChanged);
    connect(this, &AbstractSortFilterProxyModel::rowsRemoved,
            this, &AbstractSortFilterProxyModel::modelChanged);
    connect(this, &AbstractSortFilterProxyModel::layoutChanged,
            this, &AbstractSortFilterProxyModel::modelChanged);
    connect(this, &AbstractSortFilterProxyModel::modelReset,
            this, &AbstractSortFilterProxyModel::modelChanged);
}
