#ifndef DATERANGEPROXYMODEL_H
#define DATERANGEPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QDateTime>

class DateRangeProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit DateRangeProxyModel(QObject *parent = nullptr);

    Q_INVOKABLE void setDateTimeRange(QDateTime start, QDateTime end);

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const Q_DECL_OVERRIDE;

private:
    inline bool isInDateTimeRange(QDateTime &dt) const;

    QDateTime m_startDateTime;
    QDateTime m_endDateTime;
};

#endif // DATERANGEPROXYMODEL_H
