#ifndef DateRangeProxyModel_H
#define DateRangeProxyModel_H

#include <QSortFilterProxyModel>
#include <QDateTime>
#include <QDate>
#include <QSet>

#include "abstractsortfilterproxymodel.h"
#include "../common/enums.h"

class DateRangeProxyModel : public AbstractSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit DateRangeProxyModel(QObject *parent = nullptr);

    Q_INVOKABLE void setDateTimeRange(const QDateTime &startDateTime, const QDateTime &endDateTime);
    Q_INVOKABLE bool hasEvents(const QDate &date) const;

    void setSortingTypeAndRole(int role, int sortColumn = 0, Qt::SortOrder order = Qt::AscendingOrder);

signals:
    void dateRangeChanged() const;
    void hasEventsChanged(const QDate &date, bool hasEvents) const;

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QDateTime m_startDateTime;
    QDateTime m_endDateTime;

    QHash<QDate, int> m_dateEvents;

    inline bool isInDateTimeRange(const QDateTime &dt) const;

private slots:
    void onRowsInserted(const QModelIndex &parent, int first, int last);
    void onRowsRemoved(const QModelIndex &parent, int first, int last);
    void onModelReset();
};

#endif // DateRangeProxyModel_H
