#ifndef CALENDARPROXY_H
#define CALENDARPROXY_H

#include <QAbstractProxyModel>
#include <QDate>
#include <QHash>

#include "../common/enums.h"
#include "shipmentmodel.h"

class CalendarProxy : public QAbstractProxyModel
{
    Q_OBJECT
public:
    explicit CalendarProxy(QObject *parent = nullptr);

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    virtual QModelIndex parent(const QModelIndex &child) const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    virtual QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
    virtual QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;

    Q_INVOKABLE void setDateTimeRange(const QDate &start, const QDate &end);
    Q_INVOKABLE Enums::CommodityType commodityForDate(const QDate &date);
signals:

public slots:

private:
    QDate m_startDate;
    QDate m_endDate;

    QHash<QModelIndex, QModelIndex> m_sourceToProxy;
    QHash<QModelIndex, QModelIndex> m_proxyToSource;
    QHash<QDate, QModelIndex> m_dateToProxy;
    QHash<QString, QModelIndex> m_shipmentIdToModel;

    ShipmentModel* m_shipmentModel;
};

#endif // CALENDARPROXY_H
