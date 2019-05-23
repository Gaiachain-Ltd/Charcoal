#ifndef COMMODITYPROXYMODEL_H
#define COMMODITYPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QSet>
#include <QHash>

#include "abstractsortfilterproxymodel.h"
#include "../common/enums.h"

class CommodityProxyModel : public AbstractSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit CommodityProxyModel(QObject *parent = nullptr);

    Q_INVOKABLE void setFilterForType(const Enums::CommodityType commodityType, const bool enable = true);
    Q_INVOKABLE void clearCommodityFilter();
    Q_INVOKABLE bool commodityEnabled(const Enums::CommodityType commodityType) const;

    bool hasShipment(const QString& shipmentId) const;
    Enums::CommodityType shipmentCommodityType(const QString& shipmentId) const;

signals:
    void commodityTypeChanged() const;

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QSet<Enums::CommodityType> m_enabledCommodites;
    QHash<QString, Enums::CommodityType> m_shipmentsType;

private slots:
    void onRowsInserted(const QModelIndex &parent, int first, int last);
    void onRowsRemoved(const QModelIndex &parent, int first, int last);
    void onModelReset();
};

#endif // COMMODITYPROXYMODEL_H
