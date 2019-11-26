#include "modelhelper.h"

#include <QDebug>

ModelHelper::ModelHelper(QObject *parent)
    : QObject(parent)
{

}

ModelHelper &ModelHelper::instance() {
    static ModelHelper pmh;
    return pmh;
}

QVariant ModelHelper::findItem(int column, QVariant targetItem, int searchColumn, QAbstractItemModel *model) {
    for (int i = 0; i < model->rowCount(); ++i) {
        QVariant tempItem = model->data(model->index(i, column), Qt::UserRole + column + 1);

        if (tempItem == targetItem) {
            return model->data(model->index(i, searchColumn), Qt::UserRole + searchColumn + 1);
        }
    }

    return QVariant();
}

QVariantList ModelHelper::findItems(int column, QAbstractItemModel *model) {
    QVariantList dataList;

    for (int i = 0; i < model->rowCount(); ++i) {
        dataList.append(model->data(model->index(i, column), Qt::UserRole + column + 1));
    }

    return dataList;
}

QVariantList ModelHelper::findItems(int column, QVariant targetItem, int searchColumn, QAbstractItemModel *model) {
    QVariantList dataList;

    for (int i = 0; i < model->rowCount(); ++i) {
        QVariant tempItem = model->data(model->index(i, column), Qt::UserRole + column + 1);

        if (tempItem == targetItem) {
            dataList.append(model->data(model->index(i, searchColumn), Qt::UserRole + searchColumn + 1));
        }
    }

    return dataList;
}

int ModelHelper::roleNameToColumn(const QString &roleName, QAbstractItemModel *model) {
    const auto roles = model->roleNames();

    for (const int key : roles.keys()) {
        if (roles.value(key) == roleName) {
            return key - Qt::UserRole - 1;
        }
    }

    return -1;
}
