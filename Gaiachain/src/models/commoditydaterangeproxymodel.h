#ifndef COMMODITYDATERANGEPROXYMODEL_H
#define COMMODITYDATERANGEPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QDateTime>

#include "../common/enums.h"

class CommodityProxyModel;

class CommodityDateRangeProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit CommodityDateRangeProxyModel(QObject *parent = nullptr);

    Q_INVOKABLE void setDateTimeRange(QDateTime start, QDateTime end);

    void setCommodityProxyModel(CommodityProxyModel *commodityProxyModel);

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const Q_DECL_OVERRIDE;

private:
    CommodityProxyModel *m_commodityProxyModel = nullptr;

    QDateTime m_startDateTime;
    QDateTime m_endDateTime;

    bool commodityProxyModelContainsId(int shipmentId) const; //TO_DO make it faster
    inline bool isInDateTimeRange(QDateTime &dt) const;
};

#endif // COMMODITYDATERANGEPROXYMODEL_H
