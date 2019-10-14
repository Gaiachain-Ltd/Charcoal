#include "shipmenteventsproxy.h"

#include "eventmodel.h"
#include "../common/enums.h"
#include "../common/logs.h"

ShipmentEventsProxy::ShipmentEventsProxy(QObject *parent)
    : AbstractSortFilterProxyModel(parent)
{
    setSortRole(EventModel::Timestamp);
    sort(0);
}

void ShipmentEventsProxy::clearShipmentId()
{
    setShipmentId({});
}

void ShipmentEventsProxy::setShipmentId(const QString &shipmentId)
{
    if (m_shipmentId == shipmentId)
        return;

    m_shipmentId = shipmentId;
    invalidateFilter();
    emit shipmentIdChanged();
}

QVariantMap ShipmentEventsProxy::getRowAttributes(int row)
{
    static auto roleNames = sourceModel()->roleNames();

    auto rowIndex = index(row, 0);
    if (!rowIndex.isValid()) {
        qCWarning(dataModels) << "Trying to get attributes for invalid row:" << row;
        return {};
    }

    auto rowAttributes = QVariantMap{};
    for (const auto &role : roleNames.keys()) {
        rowAttributes.insert(roleNames.value(role), data(rowIndex, role));
    }
    return rowAttributes;
}

bool ShipmentEventsProxy::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    auto index = sourceModel()->index(sourceRow, 0, sourceParent);
    auto shipmentId = sourceModel()->data(index, EventModel::ShipmentId).toString();
    return  (shipmentId == m_shipmentId);
}
