#ifndef LOCALONLYPROXYMODEL_H
#define LOCALONLYPROXYMODEL_H

#include "abstractsortfilterproxymodel.h"

#include "../common/enums.h"
#include "../common/globals.h"

class LocalOnlyProxyModel : public AbstractSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit LocalOnlyProxyModel(QObject *parent = nullptr);

    Gaia::ModelData getData() const;

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
};

#endif // LOCALONLYPROXYMODEL_H
