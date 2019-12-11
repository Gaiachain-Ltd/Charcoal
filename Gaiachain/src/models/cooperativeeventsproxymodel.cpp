#include "cooperativeeventsproxymodel.h"

#include "eventmodel.h"

CooperativeEventsProxyModel::CooperativeEventsProxyModel(QObject *parent)
    : AbstractSortFilterProxyModel(parent)
{}

quint32 CooperativeEventsProxyModel::cooperativeId() const
{
    return m_cooperativeId;
}

bool CooperativeEventsProxyModel::active() const
{
    return m_active;
}

void CooperativeEventsProxyModel::setCooperativeId(quint32 cooperativeId)
{
    if (m_cooperativeId == cooperativeId) {
        return;
    }

    m_cooperativeId = cooperativeId;
    emit cooperativeIdChanged(cooperativeId);

    resetFilter();
}

void CooperativeEventsProxyModel::setActive(bool active)
{
    if (m_active == active) {
        return;
    }

    m_active = active;
    emit activeChanged(active);

    resetFilter();
}

bool CooperativeEventsProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (!m_active) {
        return true;
    }

    if (sourceModel()) {
        auto cooperativeId = sourceModel()->data(sourceModel()->index(sourceRow, 0, sourceParent), EventModel::Columns::CooperativeId).toUInt();
        return (m_cooperativeId == cooperativeId);
    }
    return false;
}
