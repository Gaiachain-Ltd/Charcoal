#ifndef ABSTRACTSORTFILTERPROXYMODEL_H
#define ABSTRACTSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class AbstractSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    AbstractSortFilterProxyModel(QObject *parent = nullptr);

signals:
    void filteringStarted() const;
    void filteringFinished() const;

    void sortingStarted() const;
    void sortingFinished() const;

    void proxyChangeStarted() const;
    void proxyChangeFinished() const;

protected:
    /**
     * @brief invalidateFilterNotify
     * Use this function instead of QSortFilterProxyModel::invalidateFilter
     * if you want to notify start and end of filtering.
     */
    void invalidateFilterNotify();

    /**
     * @brief invalidateSortNotify
     * Use this function instead of QSortFilterProxyModel::sort
     * if you want to notify start and end of sorting.
     */
    void invalidateSortNotify(int sortColumn = 0, Qt::SortOrder order = Qt::AscendingOrder);

    /**
     * @brief invalidateNotify
     * Use this function instead of QSortFilterProxyModel::invalidate
     * if you want to notify start and end of filtering and sorting.
     */
    void invalidateNotify(int sortColumn = 0);

};

#endif // ABSTRACTSORTFILTERPROXYMODEL_H
