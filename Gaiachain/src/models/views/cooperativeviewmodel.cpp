#include "cooperativeviewmodel.h"

#include "../query/cooperativeidquery.h"

CooperativeViewModel::CooperativeViewModel(QObject *parent)
    : EventModel(parent)
{}

void CooperativeViewModel::setSourceModel(SqlQueryModel *sourceModel)
{
    m_queryModel = QPointer<SqlQueryModel>(sourceModel);
    updateFilterQuery();

    AbstractReadModel::setSourceModel(sourceModel);
}

quint32 CooperativeViewModel::cooperativeId() const
{
    return m_cooperativeId;
}

bool CooperativeViewModel::cooperativeOnly() const
{
    return m_cooperativeOnly;
}

void CooperativeViewModel::setCooperativeId(quint32 cooperativeId)
{
    if (m_cooperativeId == cooperativeId) {
        return;
    }

    m_cooperativeId = cooperativeId;
    updateFilterQuery();
    emit cooperativeIdChanged(m_cooperativeId);
}

void CooperativeViewModel::setCooperativeOnly(bool cooperativeOnly)
{
    if (m_cooperativeOnly == cooperativeOnly) {
        return;
    }

    m_cooperativeOnly = cooperativeOnly;
    updateFilterQuery();
    emit cooperativeOnlyChanged(m_cooperativeOnly);
}

void CooperativeViewModel::updateFilterQuery()
{
    if (!m_queryModel.isNull()) {
        auto baseQuery = SortFilterQuery();
        if (m_cooperativeOnly) {
            m_queryModel->setSortFilterQuery(CooperativeIdQuery(m_cooperativeId, baseQuery));
        } else {
            m_queryModel->setSortFilterQuery(baseQuery);
        }
    }
}
