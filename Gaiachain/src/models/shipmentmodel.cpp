#include "shipmentmodel.h"

#include "../common/enums.h"

ShipmentModel::ShipmentModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

int ShipmentModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_data.count();
}

bool ShipmentModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role < Qt::UserRole || role >= ModelRole::LastRole)
        return false;

    int shiftedIndex = role - static_cast<int>(ModelRole::ShipmentId);
    m_data[index.row()][shiftedIndex] = value;

    emit dataChanged(index, index, {role});

    return true;
}

QVariant ShipmentModel::data(const QModelIndex &index, int role) const
{
    if (role < Qt::UserRole || role >= ModelRole::LastRole)
        return {};

    if (index.row() < 0 || index.row() > m_data.count())
        return {};

    int shiftedIndex = role - static_cast<int>(ModelRole::ShipmentId);
    auto values = m_data[index.row()];

    if (0 <= shiftedIndex && shiftedIndex < values.count())
        return values[shiftedIndex];

    Q_ASSERT(false);
    return {};
}

QHash<int, QByteArray> ShipmentModel::roleNames() const
{
    return m_roleNames;
}

void ShipmentModel::clearModel()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
}

void ShipmentModel::appendData(const Gaia::ModelData &inData)
{
    int modelCount = rowCount();
    int newDataCount = inData.count();

    beginInsertRows(QModelIndex(), modelCount, modelCount + newDataCount);

    int newIndex = modelCount;
    for (const auto &dataRow : inData) {
        QVariantList rowToInsert;
        int currentRole = ShipmentId;
        for (const auto &data : dataRow) {
            if (currentRole == LastRole)
                break;

            rowToInsert.append(data);
            ++currentRole;
        }

        while (currentRole != LastRole) {
            rowToInsert.append(QVariant());
            ++currentRole;
        }

        m_data.insert(newIndex++, rowToInsert);
    }

    endInsertRows();
}
