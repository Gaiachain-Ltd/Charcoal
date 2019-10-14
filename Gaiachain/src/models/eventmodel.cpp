#include "eventmodel.h"

#include <QDateTime>

#include "../common/logs.h"
#include "../common/enums.h"

const QHash<int, QByteArray> EventModel::sc_roleNames = {
    { ModelRole::ShipmentId,        QByteArrayLiteral("shipmentId") },
    { ModelRole::Timestamp,         QByteArrayLiteral("timestamp") },
    { ModelRole::Location,          QByteArrayLiteral("location") },
    { ModelRole::Company,           QByteArrayLiteral("company") },
    { ModelRole::UserRole,          QByteArrayLiteral("userRole") },
    { ModelRole::Action,            QByteArrayLiteral("action") },
    { ModelRole::ActionProgress,    QByteArrayLiteral("actionProgress") }
};

EventModel::EventModel(QObject *parent)
    : QAbstractListModel(parent)
{
    ModelChangedExtension::setupConnections(this);
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

    int shiftedIdx = shiftedIndex(role);
    m_data[index.row()][shiftedIdx] = value;

    emit dataChanged(index, index, {role});

    return true;
}

QVariant EventModel::data(const QModelIndex &index, int role) const
{
    if (role < Qt::UserRole || role >= ModelRole::LastRole)
        return {};

    if (index.row() < 0 || index.row() > m_data.count())
        return {};

    int shiftedIdx = shiftedIndex(role);
    auto values = m_data[index.row()];

    if (0 <= shiftedIdx && shiftedIdx < values.count())
        return values[shiftedIdx];

    Q_ASSERT(false);
    return {};
}

QHash<int, QByteArray> EventModel::roleNames() const
{
    return sc_roleNames;
}

void EventModel::clearModel()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
}

void EventModel::appendData(const Gaia::ModelData &inData)
{
    int modelCount = rowCount();
    int newDataCount = inData.count();

    beginInsertRows(QModelIndex(), modelCount, modelCount + newDataCount - 1);

    int newIndex = modelCount;
    for (const auto &dataRow : inData) {
        QVariantList rowToInsert;
        int currentRole = Qt::UserRole;
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

int EventModel::shiftedIndex(const int idx) const
{
    return idx - static_cast<int>(Qt::UserRole);
}
