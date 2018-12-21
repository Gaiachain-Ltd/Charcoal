#include "shipmentmodel.h"

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
    m_data.values(index.row())[shiftedIndex] = value;

    emit dataChanged(index, index, {role});

    return true;
}

QVariant ShipmentModel::data(const QModelIndex &index, int role) const
{
    if (role < Qt::UserRole || role >= ModelRole::LastRole)
        return QVariant();

    int shiftedIndex = role - static_cast<int>(ModelRole::ShipmentId);
    return m_data.values(index.row())[shiftedIndex];
}

QHash<int, QByteArray> ShipmentModel::roleNames() const
{
    return m_roleNames;
}
