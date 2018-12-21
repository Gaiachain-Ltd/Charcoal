#include "eventmodel.h"

#include <QDateTime>

EventModel::EventModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int EventModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_data.count();
}

bool EventModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role < Qt::UserRole || role >= ModelRole::LastRole)
        return false;

    int shiftedIndex = role - static_cast<int>(ModelRole::EventId);
    m_data.values(index.row())[shiftedIndex] = value;

    emit dataChanged(index, index, {role});

    return true;
}

QVariant EventModel::data(const QModelIndex &index, int role) const
{
    if (role < Qt::UserRole || role >= ModelRole::LastRole)
        return QVariant();

    int shiftedIndex = role - static_cast<int>(ModelRole::EventId);
    return m_data.values(index.row())[shiftedIndex];
}

QHash<int, QByteArray> EventModel::roleNames() const
{
    return m_roleNames;
}
