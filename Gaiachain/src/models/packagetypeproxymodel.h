#ifndef PACKAGETYPEPROXYMODEL_H
#define PACKAGETYPEPROXYMODEL_H

#include "abstractsortfilterproxymodel.h"

#include "../common/enums.h"

class PackageTypeProxyModel : public AbstractSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit PackageTypeProxyModel(QObject *parent = nullptr);

    Q_INVOKABLE void setPackageTypeFiltering(const Enums::PackageType &type, bool active);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QMap<Enums::PackageType, bool> m_packageTypesActivity;

    bool isActive(const Enums::PackageType &type) const;
};

#endif // PACKAGETYPEPROXYMODEL_H
