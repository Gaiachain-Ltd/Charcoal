#ifndef DATERANGEPROXYMODEL_H
#define DATERANGEPROXYMODEL_H

#include <QDateTime>
#include <QDate>
#include <QSet>

#include "abstractsortfilterproxymodel.h"

class DateRangeProxyModel : public AbstractSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit DateRangeProxyModel(QObject *parent = nullptr);

    Q_INVOKABLE void setSingleDateRange(const QDate &date);
    Q_INVOKABLE void setDateRange(const QDate &startDate, const QDate &endDate);
    Q_INVOKABLE void setDateTimeRange(const QDateTime &startDateTime, const QDateTime &endDateTime);

signals:
    void dateRangeChanged(const QDateTime &startDateTime, const QDateTime &endDateTime) const;

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QDateTime m_startDateTime;
    QDateTime m_endDateTime;

    inline bool isInDateTimeRange(const QDateTime &dt) const;
};

#endif // DATERANGEPROXYMODEL_H
