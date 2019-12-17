#include "localviewmodel.h"

#include "../query/cooperativeidquery.h"
#include "../query/sorttimestampquery.h"
#include "../query/localeventsquery.h"

LocalViewModel::LocalViewModel(QObject *parent)
    : CooperativeViewModel(parent)
{}

void LocalViewModel::updateFilterQuery()
{
    if (!m_queryModel.isNull()) {
        auto baseQuery = LocalEventsQuery(SortTimestampQuery(SortFilterQuery()));
        if (m_cooperativeOnly) {
            m_queryModel->setSortFilterQuery(CooperativeIdQuery(m_cooperativeId, baseQuery));
        } else {
            m_queryModel->setSortFilterQuery(baseQuery);
        }
    }
}
