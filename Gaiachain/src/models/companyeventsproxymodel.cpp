#include "companyeventsproxymodel.h"

#include "eventmodel.h"

CompanyEventsProxyModel::CompanyEventsProxyModel(QObject *parent)
    : AbstractSortFilterProxyModel(parent)
{
    setFilterRole(EventModel::CooperativeId);
    connect(this, &CompanyEventsProxyModel::activeChanged, this, [this]() { invalidateFilter(); });
}

void CompanyEventsProxyModel::setCompanyId(const QString &companyId)
{
    setFilterFixedString(companyId);
}

bool CompanyEventsProxyModel::active() const
{
    return m_active;
}

void CompanyEventsProxyModel::setActive(bool active)
{
    if (m_active == active) {
        return;
    }

    m_active = active;
    emit activeChanged(m_active);
}

bool CompanyEventsProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (!m_active) {
        return true;
    }

    return AbstractSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
}
