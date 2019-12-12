#ifndef PACKAGELASTACTIONPROXYMODEL_H
#define PACKAGELASTACTIONPROXYMODEL_H

#include "abstractsortfilterproxymodel.h"

#include "../common/enums.h"
#include "../common/globals.h"

class PackageLastActionProxyModel : public AbstractSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit PackageLastActionProxyModel(const Enums::SupplyChainAction &lastAction, QObject *parent = nullptr);

    void setSourceModel(QAbstractItemModel *sourceModel) override;

    Gaia::ModelData getData() const;

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    Enums::SupplyChainAction m_lastAction = Enums::SupplyChainAction::Unknown;

};

#endif // PACKAGELASTACTIONPROXYMODEL_H
