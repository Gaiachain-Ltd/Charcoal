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

    Q_INVOKABLE void setCommodityType(Enums::CommodityType filterType, bool enable = true);
    Q_INVOKABLE bool commodityEnabled(Enums::CommodityType filterType) const;

    bool hasShipmentId(const QString& id) const;

signals:
    void commodityTypeChanged() const;
    void filteringFinished() const;

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QSet<Enums::CommodityType> m_enabledCommodites;
    mutable QSet<QString> m_shipmentIds;
};

#endif // COMMODITYPROXYMODEL_H
