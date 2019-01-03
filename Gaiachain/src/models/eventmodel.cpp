#include "eventmodel.h"

#include <QDateTime>

#include "../common/enums.h"

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
    m_data[index.row()][shiftedIndex] = value;

    emit dataChanged(index, index, {role});

    return true;
}

QVariant EventModel::data(const QModelIndex &index, int role) const
{
    if (role < Qt::UserRole || role >= ModelRole::LastRole)
        return {};

    if (index.row() < 0 || index.row() > m_data.count())
        return {};

    int shiftedIndex = role - static_cast<int>(ModelRole::EventId);
    auto values = m_data[index.row()];

    if (0 <= shiftedIndex && shiftedIndex < values.count())
        return values[shiftedIndex];

    Q_ASSERT(false);
    return {};
}

QHash<int, QByteArray> EventModel::roleNames() const
{
    return m_roleNames;
}

void EventModel::appendData(const Gaia::ModelData &inData)
{
    int modelCount = rowCount();
    int newDataCount = inData.count();

    beginInsertRows(QModelIndex(), modelCount, modelCount + newDataCount);

    int newIndex = modelCount;
    for (const auto &dataRow : inData) {
        QVariantList rowToInsert;
        int currentRole = EventId;
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
