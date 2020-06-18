#ifndef CALENDARVIEWMODEL_H
#define CALENDARVIEWMODEL_H

#include "cooperativeviewmodel.h"

#include <QDate>

class CalendarViewModel : public CooperativeViewModel
{
    Q_OBJECT
    Q_PROPERTY(QDate fromDate READ fromDate WRITE setFromDate NOTIFY fromDateChanged)
    Q_PROPERTY(QDate toDate READ toDate WRITE setToDate NOTIFY toDateChanged)

public:
    CalendarViewModel(QObject *parent = nullptr);

    QDate fromDate() const;
    QDate toDate() const;

    QDateTime fromDateTime() const;
    QDateTime toDateTime() const;

    Q_INVOKABLE void setDateRange(const QDate &fromDate, const QDate &toDate);
    Q_INVOKABLE void setDate(const QDate &date);

    Q_INVOKABLE void clear();

public slots:
    void setFromDate(const QDate &fromDate);
    void setToDate(const QDate &toDate);

signals:
    void fromDateChanged(const QDate &fromDate);
    void toDateChanged(const QDate &toDate);

private:
    QDate m_fromDate;
    QDate m_toDate;

    void updateFilterQuery() override;

    template <typename BaseQuery>
    void updateFilterQuery(BaseQuery &&baseQuery);
};

#endif // CALENDARVIEWMODEL_H
