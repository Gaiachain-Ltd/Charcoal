#include "latestrangeeventsproxymodel.h"

#include "eventmodel.h"

LatestRangeEventsProxyModel::LatestRangeEventsProxyModel(QObject *parent)
    : AbstractSortFilterProxyModel(parent)
{
    AbstractSortFilterProxyModel::setSourceModel(&m_latestEventsModel);
}

void LatestRangeEventsProxyModel::clearRowCount()
{
    m_availableEvents = 0;
    invalidateFilter();
    fetchMore({});
}

void LatestRangeEventsProxyModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    m_availableEvents = 0;
    m_latestEventsModel.setSourceModel(sourceModel);
}

bool LatestRangeEventsProxyModel::canFetchMore(const QModelIndex &parent) const
{
    if (sender()) {
        // TODO: better solution; This is done to avoid fetching all date because of layoutChange after sorting
        return false;
    }

    const auto rowsAvailable = AbstractSortFilterProxyModel::rowCount(parent);
    return (m_availableEvents < rowsAvailable + sc_fetchSize);
}

void LatestRangeEventsProxyModel::fetchMore(const QModelIndex &parent)
{
    qDebug() << "Fetching events:" << m_availableEvents << m_availableEvents + sc_fetchSize;
    emit fetchEvents(sc_fetchSize, oldestEventDate());

    const auto rowsAvailable = AbstractSortFilterProxyModel::rowCount(parent);
    auto hasMoreData = (rowsAvailable > m_availableEvents);

    m_availableEvents += sc_fetchSize;
    if (hasMoreData) {
        invalidateFilter();
    }
}

QDateTime LatestRangeEventsProxyModel::newestEventDate() const
{
    return rowCount() ? data(index(0, 0), EventModel::Timestamp).toDateTime()
                      : QDateTime{};
}

QDateTime LatestRangeEventsProxyModel::oldestEventDate() const
{
    return rowCount() ? data(index(rowCount() - 1, 0), EventModel::Timestamp).toDateTime()
                      : QDateTime{};
}

bool LatestRangeEventsProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &) const
{
    return (sourceRow <= m_availableEvents);
}
