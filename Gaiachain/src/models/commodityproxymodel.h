#ifndef COMMODITYPROXYMODEL_H
#define COMMODITYPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QSet>

#include "../common/enums.h"

class CommodityProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit CommodityProxyModel(QObject *parent = nullptr);

    Q_INVOKABLE void setCommodityType(Enums::CommodityType filterType, bool enable = true);
    Q_INVOKABLE bool commodityEnabled(Enums::CommodityType filterType) const;

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const Q_DECL_OVERRIDE;

signals:
    void commodityTypeChanged() const;
    void filteringFinished() const;

private:
    QSet<Enums::CommodityType> m_enabledCommodites;
};

#endif // COMMODITYPROXYMODEL_H
