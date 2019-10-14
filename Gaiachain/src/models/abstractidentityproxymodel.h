#ifndef ABSTRACTIDENTITYPROXYMODEL_H
#define ABSTRACTIDENTITYPROXYMODEL_H

#include <QIdentityProxyModel>
#include "modelchangedextension.h"

class AbstractIdentityProxyModel : public QIdentityProxyModel, public ModelChangedExtension
{
    Q_OBJECT
    Q_INTERFACES(ModelChangedExtension)

public:
    AbstractIdentityProxyModel(QObject *parent = nullptr);

signals:
    void modelChanged() const override;
};

#endif // ABSTRACTIDENTITYPROXYMODEL_H
