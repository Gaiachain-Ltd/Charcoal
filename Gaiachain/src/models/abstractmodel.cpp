#include "abstractmodel.h"

AbstractModel::AbstractModel(QObject *parent)
    : QAbstractListModel(parent)
{
    ModelChangedExtension::setupConnections(this);
}

int AbstractModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_data.count();
}

bool AbstractModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role < Qt::UserRole || role >= lastRole())
        return false;

    if (index.row() < 0 || index.row() > m_data.count())
        return {};

    m_data[index.row()][roleShift(role)] = value;
    emit dataChanged(index, index, {role});
    return true;
}

QVariant AbstractModel::data(const QModelIndex &index, int role) const
{
    if (role < Qt::UserRole || role >= lastRole())
        return {};

    if (index.row() < 0 || index.row() > m_data.count())
        return {};

    auto shiftedRole = roleShift(role);
    auto values = m_data[index.row()];
    if (0 <= shiftedRole && shiftedRole < values.count()) {
        return values[shiftedRole];
    }

    Q_ASSERT(false);
    return {};
}

void AbstractModel::clearModel()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
}

void AbstractModel::appendData(const Gaia::ModelData &inData)
{
    int modelCount = rowCount();
    int newDataCount = inData.count();

    beginInsertRows(QModelIndex(), modelCount, modelCount + newDataCount - 1);

    int newIndex = modelCount;
    for (const auto &dataRow : inData) {
        QVariantList rowToInsert;
        int currentRole = Qt::UserRole;
        for (const auto &data : dataRow) {
            if (currentRole == lastRole())
                break;

            rowToInsert.append(data);
            ++currentRole;
        }

        while (currentRole != lastRole()) {
            rowToInsert.append(QVariant());
            ++currentRole;
        }

        m_data.insert(newIndex++, rowToInsert);
    }

    endInsertRows();
}

int AbstractModel::roleShift(const int role) const
{
    return role - static_cast<int>(Qt::UserRole);
}
