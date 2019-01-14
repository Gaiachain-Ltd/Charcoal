#ifndef COMMODITYDATERANGEPROXYMODEL_H
#define COMMODITYDATERANGEPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QDateTime>
#include <QDate>
#include <QSet>

#include "abstractsortfilterproxymodel.h"
#include "../common/enums.h"

class CommodityProxyModel;

class CommodityDateRangeProxyModel : public AbstractSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit CommodityDateRangeProxyModel(QObject *parent = nullptr);

    Q_INVOKABLE void setDateTimeRange(const QDateTime &startDateTime, const QDateTime &endDateTime);
    Q_INVOKABLE bool hasEvents(const QDate &date) const;
    Q_INVOKABLE QList<Enums::CommodityType> eventsCommodityTypes(const QDate &date) const;

    void setCommodityProxyModel(CommodityProxyModel *commodityProxyModel);
    void setSortingTypeAndRole(int role, int sortColumn = 0, Qt::SortOrder order = Qt::AscendingOrder);

signals:
    void dateRangeChanged() const;
    void eventsCommoditiesChanged(const QDate &date) const;

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    CommodityProxyModel *m_commodityProxyModel = nullptr;

    QDateTime m_startDateTime;
    QDateTime m_endDateTime;

    QHash<QDate, QHash<Enums::CommodityType, int>> m_dateEventsCommodityTypes; // should NOT contains CommodityTypes of 0 count!

    inline bool isInDateTimeRange(QDateTime &dt) const;

private slots:
    void onRowsInserted(const QModelIndex &parent, int first, int last);
    void onRowsRemoved(const QModelIndex &parent, int first, int last);
    void onModelReset();
};

#endif // COMMODITYDATERANGEPROXYMODEL_H
