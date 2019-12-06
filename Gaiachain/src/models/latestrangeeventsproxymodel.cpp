#include "latestrangeeventsproxymodel.h"

#include "eventmodel.h"

LatestRangeEventsProxyModel::LatestRangeEventsProxyModel(QObject *parent)
    : AbstractSortFilterProxyModel(parent)
{
    AbstractSortFilterProxyModel::setSourceModel(&m_latestEventsModel);
}

void LatestRangeEventsProxyModel::clearRowCount()
{
    m_offset = 0;
    invalidateFilter();
    fetchMore({});
}

void LatestRangeEventsProxyModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    m_offset = 0;
    m_latestEventsModel.setSourceModel(sourceModel);
}

bool LatestRangeEventsProxyModel::canFetchMore(const QModelIndex &parent) const
{
    if (sender()) {
        // TODO: better solution; This is done to avoid fetching all date because of layoutChange after sorting
        return false;
    }

    const auto rowsAvailable = AbstractSortFilterProxyModel::rowCount(parent);
    return (m_offset < rowsAvailable + sc_fetchSize);
}

void LatestRangeEventsProxyModel::fetchMore(const QModelIndex &parent)
{
    qDebug() << "Fetching events:" << m_offset << m_offset + sc_fetchSize;
    emit fetchEvents(sc_fetchSize, m_offset);

    const auto rowsAvailable = AbstractSortFilterProxyModel::rowCount(parent);
    auto hasMoreData = (rowsAvailable > m_offset);

    m_offset += sc_fetchSize;
    if (hasMoreData) {
        invalidateFilter();
    }
}

bool LatestRangeEventsProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &) const
{
    return (sourceRow <= m_offset);
}
