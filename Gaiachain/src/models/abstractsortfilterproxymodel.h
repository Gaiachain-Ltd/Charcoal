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

protected:
    /**
     * @brief _invalidateFilter
     * Use this function instead of QSortFilterProxyModel::invalidateFilter
     * if you want to notify end of filtering with use of filteringFinished signal.
     */
    void invalidateFilterNotify();

};

#endif // ABSTRACTSORTFILTERPROXYMODEL_H
