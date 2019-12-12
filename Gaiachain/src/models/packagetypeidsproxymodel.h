#ifndef PACKAGETYPEIDSPROXYMODEL_H
#define PACKAGETYPEIDSPROXYMODEL_H

#include "abstractsortfilterproxymodel.h"

#include <QSet>

#include "../common/enums.h"

class PackageTypeIdsProxyModel : public AbstractSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit PackageTypeIdsProxyModel(QObject *parent = nullptr);

    Q_INVOKABLE void setPackageType(const Enums::PackageType &type);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    Enums::PackageType m_packageType;
    Enums::SupplyChainAction m_creationAction;
};

#endif // PACKAGETYPEIDSPROXYMODEL_H
