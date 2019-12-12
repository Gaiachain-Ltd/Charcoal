#include "abstractidentityproxymodel.h"

AbstractIdentityProxyModel::AbstractIdentityProxyModel(QObject *parent)
    : QIdentityProxyModel(parent)
{
    ModelChangedExtension::setupConnections(this);
}
