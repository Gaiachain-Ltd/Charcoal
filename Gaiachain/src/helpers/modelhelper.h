#ifndef MODELHELPER_H
#define MODELHELPER_H

#include <QObject>
#include "../models/abstractreadmodel.h"

class ModelHelper : public QObject {
    Q_OBJECT
public:
    static ModelHelper &instance();

    Q_INVOKABLE static QVariant getData(int row, const QString &roleName, QAbstractItemModel *model);

    Q_INVOKABLE static int findRow(const QString &roleName, const QVariant &targetItem, QAbstractItemModel *model);

    Q_INVOKABLE static QVariant findItem(const QString &roleName, const QVariant &targetItem,
                                  const QString &searchRoleName, QAbstractItemModel *model);
    Q_INVOKABLE static QVariantList findItem(const QString &roleName, const QVariant &targetItem,
                                      const QStringList &searchRoleNames, QAbstractItemModel *model);

    Q_INVOKABLE static QVariantList findItems(const QString &roleName, const QVariant &targetItem,
                                   const QString &searchRoleName, QAbstractItemModel *model);

    Q_INVOKABLE static int roleNameToNumber(const QString &roleName, QAbstractItemModel *model);

private:
    ModelHelper(QObject *parent = nullptr);

};

#endif // PRODUCERSMODELHELPER_H
