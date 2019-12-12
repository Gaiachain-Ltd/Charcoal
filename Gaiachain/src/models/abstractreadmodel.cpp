#include "abstractreadmodel.h"

#include <QSqlQueryModel>

#include "../common/types.h"

AbstractReadModel::AbstractReadModel(QObject *parent)
    : QIdentityProxyModel(parent)
{
    ModelChangedExtension::setupConnections(this);
}

void AbstractReadModel::setSourceModel(QSqlQueryModel *sourceModel)
{
    QIdentityProxyModel::setSourceModel(sourceModel);
}

int AbstractReadModel::firstColumn() const
{
    return 0;
}

QVariant AbstractReadModel::data(const QModelIndex &index, int role) const
{
    if (role >= Qt::UserRole) {
        if (columnShift(role) < firstColumn() || columnShift(role) >= lastColumn()) {
            return {};
        }
    } else {
        if (index.column() < firstColumn() || index.column() >= lastColumn()) {
            return QIdentityProxyModel::data(index);
        }
    }

    auto column = role >= Qt::UserRole ? columnShift(role) : index.column();
    auto updatedIndex = this->index(index.row(), column, index.parent());

    auto value = QIdentityProxyModel::data(updatedIndex);
    types::convert(value, roleAppTypes().value(roleShift(column)));

    return value;
}

QModelIndexList AbstractReadModel::match(const QModelIndex &start, int role, const QVariant &value, int hits, Qt::MatchFlags flags) const
{
    auto column = role >= Qt::UserRole ? columnShift(role) : start.column();
    auto updatedStart = this->index(start.row(), column, start.parent());
    return QIdentityProxyModel::match(updatedStart, Qt::DisplayRole, value, hits, flags);
}

QModelIndexList AbstractReadModel::find(const Gaia::ModelEntry &entryData)
{
    const int FirstColumn = firstColumn();
    const int LastColumn = lastColumn();

    auto dataIndexes = QModelIndexList{};
    if (!entryData.isEmpty()) {
        auto column = FirstColumn;
        while(entryData.value(column - FirstColumn).isNull() &&
              column < LastColumn && (column - FirstColumn) < entryData.count()) {
            ++column;
        }

        if (column < LastColumn && (column - FirstColumn) < entryData.count()) {
            dataIndexes = match(index(0, 0), roleShift(column), entryData.value(column - FirstColumn), -1, Qt::MatchExactly);
        }

        auto searchIndexes = QModelIndexList{};
        for (auto searchColumn = column + 1; searchColumn < LastColumn && (searchColumn - FirstColumn) < entryData.count(); ++searchColumn) {
            const auto role = roleShift(column);
            const auto entryValue = entryData.value(column - FirstColumn);
            std::copy_if(dataIndexes.constBegin(), dataIndexes.constEnd(), std::back_inserter(searchIndexes),
                         [this, &role, &entryValue](const auto &index) {
                return (this->data(index, role) == entryValue);
            });

            dataIndexes = searchIndexes;
            if (dataIndexes.isEmpty()) {
                break;
            }
        }
    }

    return dataIndexes;
}

Gaia::ModelEntry AbstractReadModel::getEntry(int row) const
{
    const int FirstColumn = firstColumn();
    const int LastColumn = lastColumn();

    auto entryData = Gaia::ModelEntry{};
    for (auto column = FirstColumn; column < LastColumn; ++column) {
        const auto role = roleShift(column);
        entryData.append(data(index(row, 0), role));
    }

    return entryData;
}

Gaia::ModelData AbstractReadModel::getData() const
{
    auto modelData = Gaia::ModelData{};
    for (auto row = 0; row < rowCount(); ++row) {
        modelData.append(getEntry(row));
    }
    return modelData;
}

int AbstractReadModel::columnShift(const int &role) const
{
    return role - Qt::UserRole;
}

int AbstractReadModel::roleShift(const int &column) const
{
    return column + Qt::UserRole;
}
