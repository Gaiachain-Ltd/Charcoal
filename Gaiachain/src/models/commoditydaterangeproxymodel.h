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

    void setCommodityProxyModel(CommodityProxyModel *commodityProxyModel);

    Q_INVOKABLE void setDateTimeRange(QDateTime start, QDateTime end);
    Q_INVOKABLE void setCommodityType(Enums::CommodityType filterType);

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const Q_DECL_OVERRIDE;

private:
    CommodityProxyModel *m_commodityProxyModel = nullptr;

    bool commodityProxyModelContainsId(int shipmentId) const; //TO_DO make it faster
};

#endif // COMMODITYDATERANGEPROXYMODEL_H
