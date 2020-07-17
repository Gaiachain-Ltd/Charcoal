#include "modelhelper.h"

#include <QAbstractItemModel>

ModelHelper::ModelHelper(QObject *parent)
    : QObject(parent)
{}

ModelHelper &ModelHelper::instance()
{
    static ModelHelper pmh;
    return pmh;
}

QVariant ModelHelper::getData(int row, const QString &roleName, QAbstractItemModel *model)
{
    if (model) {
        return model->data(model->index(row, 0), roleNameToNumber(roleName, model));
    }
    return {};
}

int ModelHelper::findRow(const QString &roleName, const QVariant &targetItem, QAbstractItemModel *model)
{
    if (model) {
        auto matchingIndex = model->match(model->index(0, 0), roleNameToNumber(roleName, model), targetItem, 1, Qt::MatchExactly);
        return !matchingIndex.isEmpty() ? matchingIndex.first().row() : -1;
    }
    return -1;
}

QVariant ModelHelper::findItem(const QString &roleName, const QVariant &targetItem, const QString &searchRoleName, QAbstractItemModel *model)
{
    auto items = findItems(roleName, targetItem, searchRoleName, model);
    return items.size() ? items.first() : QVariant{};
}

QVariantList ModelHelper::findItem(const QString &roleName, const QVariant &targetItem, const QStringList &searchRoleNames, QAbstractItemModel *model)
{
    auto dataList = QVariantList{};
    if (model) {
        auto matchingIndex = model->match(model->index(0, 0), roleNameToNumber(roleName, model), targetItem, 1, Qt::MatchExactly);
        if (!matchingIndex.isEmpty()) {
            const auto row = matchingIndex.first().row();

            for (const auto &roleName : searchRoleNames) {
                auto data = model->data(model->index(row, 0), roleNameToNumber(roleName, model));
                dataList.append(data);
            }
        }
    }

    return dataList;
}

QVariantList ModelHelper::findItems(const QString &roleName, const QVariant &targetItem, const QString &searchRoleName, QAbstractItemModel *model)
{
    auto dataList = QVariantList{};

    if (model) {
        const auto matchingIndexes = model->match(model->index(0, 0), roleNameToNumber(roleName, model), targetItem, 1, Qt::MatchExactly);
        for (const auto &matchingIndex : matchingIndexes) {
            const auto data = model->data(model->index(matchingIndex.row(), 0), roleNameToNumber(searchRoleName, model));
            dataList.append(data);
        }
    }

    return dataList;
}

int ModelHelper::roleNameToNumber(const QString &roleName, QAbstractItemModel *model)
{
    if (model) {
        const auto roles = model->roleNames();
        return roles.key(roleName.toLatin1(), -1);
    }
    return -1;
}
