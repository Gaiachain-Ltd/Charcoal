#ifndef CREATEDHARVESTIDSPROXYMODEL_H
#define CREATEDHARVESTIDSPROXYMODEL_H

#include "abstractsortfilterproxymodel.h"

class CreatedHarvestIdsProxyModel : public AbstractSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit CreatedHarvestIdsProxyModel(QObject *parent = nullptr);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
};

#endif // CREATEDHARVESTIDSPROXYMODEL_H
