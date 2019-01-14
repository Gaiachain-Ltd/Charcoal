#ifndef ABSTRACTSORTFILTERPROXYMODEL_H
#define ABSTRACTSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class AbstractSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    AbstractSortFilterProxyModel(QObject *parent = nullptr);

signals:
    void modelChanged() const;
};

#endif // ABSTRACTSORTFILTERPROXYMODEL_H
