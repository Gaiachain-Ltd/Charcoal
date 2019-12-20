#ifndef LATESTRANGEEVENTSPROXYMODEL_H
#define LATESTRANGEEVENTSPROXYMODEL_H

#include "abstractsortfilterproxymodel.h"

class LatestRangeEventsProxyModel : public AbstractSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit LatestRangeEventsProxyModel(QObject *parent = nullptr);

    Q_INVOKABLE void clearRowCount();

    void setSourceModel(QAbstractItemModel *sourceModel) override;

    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

signals:
    void fetchEvents(int number, int offset) const;

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    const static int sc_fetchSize = 20;
    int m_offset = 0;
};

#endif // LATESTRANGEEVENTSPROXYMODEL_H
