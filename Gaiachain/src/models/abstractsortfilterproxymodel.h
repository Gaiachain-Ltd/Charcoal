#ifndef ABSTRACTSORTFILTERPROXYMODEL_H
#define ABSTRACTSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "modelchangedextension.h"

class AbstractSortFilterProxyModel : public QSortFilterProxyModel, public ModelChangedExtension
{
    Q_OBJECT
    Q_INTERFACES(ModelChangedExtension)
    Q_PROPERTY(int size READ rowCount NOTIFY modelChanged)

public:
    AbstractSortFilterProxyModel(QObject *parent = nullptr);

signals:
    void modelChanged() const override;
};

#endif // ABSTRACTSORTFILTERPROXYMODEL_H
