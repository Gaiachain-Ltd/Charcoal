#ifndef RELATIONSLISTPROXYMODEL_H
#define RELATIONSLISTPROXYMODEL_H

#include "abstractidentityproxymodel.h"

#include "../common/packagedata.h"
#include "../common/enums.h"

class RelationsListProxyModel : public AbstractIdentityProxyModel
{
    Q_OBJECT

public:
    explicit RelationsListProxyModel(QObject *parent = nullptr);

    QStringList relatedPackages(const QString &packageId) const;
};

#endif // RELATIONSLISTPROXYMODEL_H
