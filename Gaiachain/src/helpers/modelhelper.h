#ifndef MODELHELPER_H
#define MODELHELPER_H

#include <QObject>
#include <QAbstractItemModel>

class ModelHelper : public QObject {
    Q_OBJECT
public:
    static ModelHelper &instance();

    Q_INVOKABLE QVariant findItem(int column, QVariant targetItem, int searchColumn, QAbstractItemModel *model);
    Q_INVOKABLE QVariantList findItems(int column, QAbstractItemModel *model);
    Q_INVOKABLE QVariantList findItems(int column, QVariant targetItem, int searchColumn, QAbstractItemModel *model);
    Q_INVOKABLE int roleNameToColumn(const QString &roleName, QAbstractItemModel *model);

private:
    ModelHelper(QObject *parent = nullptr);
};

#endif // PRODUCERSMODELHELPER_H
