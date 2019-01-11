#include "calendarproxy.h"

#include <QDebug>

#include "eventmodel.h"
#include "shipmentmodel.h"

CalendarProxy::CalendarProxy(QObject *parent)
    : QAbstractProxyModel(parent)
{

}

QModelIndex CalendarProxy::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return createIndex(row, column);
}

QModelIndex CalendarProxy::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)

    return {};
}

int CalendarProxy::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 0;
}

int CalendarProxy::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 0;
}

QVariant CalendarProxy::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(index)
    Q_UNUSED(role)

    return {};
}

QModelIndex CalendarProxy::mapToSource(const QModelIndex &proxyIndex) const
{
    return m_proxyToSource[proxyIndex];
}

QModelIndex CalendarProxy::mapFromSource(const QModelIndex &sourceIndex) const
{
    return m_sourceToProxy[sourceIndex];
}

void CalendarProxy::setDateTimeRange(const QDate &start, const QDate &end)
{
    m_startDate = start;
    m_endDate = end;

    m_sourceToProxy.clear();
    m_proxyToSource.clear();
    m_dateToProxy.clear();
    m_shipmentIdToModel.clear();

    // Events model indexes mapping
    auto srcModel = sourceModel();
    auto srcCount = srcModel->rowCount();
    int proxyRow = 0;
    for (int i = 0; i < srcCount; ++i) {
        auto srcIdx = srcModel->index(i, 0);
        auto eventDate = srcModel->data(srcIdx, EventModel::Timestamp).toDate();
        if (m_startDate > eventDate || eventDate > m_endDate)
            continue;

        auto proxyIdx = index(proxyRow++, 0);
        m_dateToProxy.insert(eventDate, proxyIdx);
        m_proxyToSource.insert(proxyIdx, srcIdx);
        m_sourceToProxy.insert(srcIdx, proxyIdx);

        m_shipmentIdToModel.insert(srcModel->data(srcIdx, EventModel::ShipmentId).toString(), {});
    }

    // Shipment model indexes mapping
    auto shipmentCount = m_shipmentModel->rowCount();
    for (int i=0; i < shipmentCount; ++i) {
        auto shipmentIdx = m_shipmentModel->index(i, 0);
        auto shipmentId = m_shipmentModel->data(shipmentIdx, ShipmentModel::ShipmentId).toString();
        if (!m_shipmentIdToModel.contains(shipmentId))
            continue;

        m_shipmentIdToModel[shipmentId] = shipmentIdx;
   }
}

Enums::CommodityType CalendarProxy::commodityForDate(const QDate &date)
{
    if (!m_dateToProxy.contains(date))
        return Enums::CommodityType::InvalidCommodity;

    auto proxyIdx = m_dateToProxy[date];
    auto srcIdx = mapToSource(proxyIdx);

    auto shipmentId = sourceModel()->data(srcIdx, EventModel::ShipmentId).toString();

    auto shipmentIdx = m_shipmentIdToModel[shipmentId];
    auto commodityType = m_shipmentModel->data(shipmentIdx, ShipmentModel::Commodity).toInt();

    qDebug() << proxyIdx << srcIdx << shipmentId << shipmentIdx << commodityType;

    return static_cast<Enums::CommodityType>(commodityType);
}
