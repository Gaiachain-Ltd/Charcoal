#include "parcelviewmodel.h"

#include "../proxy/produceridquery.h"

ParcelViewModel::ParcelViewModel(QObject *parent)
    : ParcelModel(parent)
{}

quint32 ParcelViewModel::producerId() const
{
    return m_producerId;
}

void ParcelViewModel::setSourceModel(SqlQueryModel *sourceModel)
{
    m_queryModel = QPointer<SqlQueryModel>(sourceModel);
    updateFilterQuery();

    AbstractReadModel::setSourceModel(sourceModel);
}

void ParcelViewModel::setProducerId(quint32 producerId)
{
    if (m_producerId == producerId)
        return;

    m_producerId = producerId;

    updateFilterQuery();
    emit producerIdChanged(m_producerId);
}

void ParcelViewModel::updateFilterQuery()
{
    if (!m_queryModel.isNull()) {
        m_queryModel->setSortFilterQuery(ProducerIdQuery(m_producerId, SortFilterQuery()));
    }
}
