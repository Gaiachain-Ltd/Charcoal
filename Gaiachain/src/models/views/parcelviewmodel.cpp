#include "parcelviewmodel.h"

#include "../proxy/produceridquery.h"

ParcelViewModel::ParcelViewModel(QObject *parent)
    : ParcelModel(parent)
{}

quint32 ParcelViewModel::producerId() const
{
    return m_producerId;
}

bool ParcelViewModel::allProducers() const
{
    return m_allProducers;
}

void ParcelViewModel::setSourceModel(SqlQueryModel *sourceModel)
{
    m_queryModel = QPointer<SqlQueryModel>(sourceModel);
    updateFilterQuery();

    AbstractReadModel::setSourceModel(sourceModel);
}

void ParcelViewModel::setProducerId(quint32 producerId)
{
    if (m_producerId == producerId) {
        return;
    }

    m_producerId = producerId;

    updateFilterQuery();
    emit producerIdChanged(m_producerId);
}

void ParcelViewModel::setAllProducers(bool allProducers)
{
    if (m_allProducers == allProducers) {
        return;
    }

    m_allProducers = allProducers;

    updateFilterQuery();
    emit allProducersChanged(m_allProducers);
}

void ParcelViewModel::updateFilterQuery()
{
    if (!m_queryModel.isNull()) {
        if (m_allProducers) {
            m_queryModel->setSortFilterQuery(SortFilterQuery());
        } else {
            m_queryModel->setSortFilterQuery(ProducerIdQuery(m_producerId, SortFilterQuery()));
        }
    }
}
