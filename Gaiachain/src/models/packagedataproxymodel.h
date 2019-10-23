#ifndef PACKAGEDATAPROXYMODEL_H
#define PACKAGEDATAPROXYMODEL_H

#include "abstractidentityproxymodel.h"

#include "../common/packagedata.h"
#include "../common/enums.h"

class PackageDataProxyModel : public AbstractIdentityProxyModel
{
    Q_OBJECT

public:
    explicit PackageDataProxyModel(QObject *parent = nullptr);

    void fillPackageData(PackageData &packageData) const;
};

#endif // PACKAGEDATAPROXYMODEL_H
