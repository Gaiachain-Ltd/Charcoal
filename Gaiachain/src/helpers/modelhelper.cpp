#include "modelhelper.h"

ModelHelper::ModelHelper(QObject *parent)
    : QObject(parent)
{}

ModelHelper &ModelHelper::instance()
{
    static ModelHelper pmh;
    return pmh;
}

QVariant ModelHelper::getData(int row, const QString &roleName, AbstractReadModel *model)
{
    if (model) {
        return model->data(model->index(row, 0), roleNameToNumber(roleName, model));
    }
    return {};
}

QVariant ModelHelper::findItem(const QString &roleName, const QVariant &targetItem, const QString &searchRoleName, AbstractReadModel *model)
{
    if (model) {
        auto matchingIndex = model->match(model->index(0, 0), roleNameToNumber(roleName, model), targetItem, 1, Qt::MatchExactly);
        if (!matchingIndex.isEmpty()) {
            return model->data(model->index(matchingIndex.first().row(), 0), roleNameToNumber(searchRoleName, model));
        }
    }

    return {};
}

QVariantList ModelHelper::findItem(const QString &roleName, const QVariant &targetItem, const QStringList &searchRoleNames, AbstractReadModel *model)
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

int ModelHelper::roleNameToNumber(const QString &roleName, AbstractReadModel *model)
{
    if (model) {
        const auto roles = model->roleNames();
        return roles.key(roleName.toLatin1(), -1);
    }
    return -1;
}
