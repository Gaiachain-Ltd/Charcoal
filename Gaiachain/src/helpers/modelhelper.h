#ifndef MODELHELPER_H
#define MODELHELPER_H

#include <QObject>
#include "../models/abstractreadmodel.h"

class ModelHelper : public QObject {
    Q_OBJECT
public:
    static ModelHelper &instance();

    Q_INVOKABLE QVariant getData(int row, const QString &roleName, AbstractReadModel *model);
    Q_INVOKABLE QVariant findItem(const QString &roleName, const QVariant &targetItem,
                                  const QString &searchRoleName, AbstractReadModel *model);
    Q_INVOKABLE QVariantList findItem(const QString &roleName, const QVariant &targetItem,
                                      const QStringList &searchRoleNames, AbstractReadModel *model);

private:
    ModelHelper(QObject *parent = nullptr);

    int roleNameToNumber(const QString &roleName, AbstractReadModel *model);
};

#endif // PRODUCERSMODELHELPER_H
