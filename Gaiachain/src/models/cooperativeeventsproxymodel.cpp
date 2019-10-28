#include "cooperativeeventsproxymodel.h"

#include "eventmodel.h"

CooperativeEventsProxyModel::CooperativeEventsProxyModel(QObject *parent)
    : AbstractSortFilterProxyModel(parent)
{
    setFilterRole(EventModel::CooperativeId);
    connect(this, &CooperativeEventsProxyModel::activeChanged, this, [this]() { invalidateFilter(); });
}

void CooperativeEventsProxyModel::setCooperativeId(const QString &cooperativeId)
{
    setFilterFixedString(cooperativeId);
}

bool CooperativeEventsProxyModel::active() const
{
    return m_active;
}

void CooperativeEventsProxyModel::setActive(bool active)
{
    if (m_active == active) {
        return;
    }

    m_active = active;
    emit activeChanged(m_active);
}

bool CooperativeEventsProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (!m_active) {
        return true;
    }

    return AbstractSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
}
