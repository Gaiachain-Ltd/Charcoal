#ifndef COMMODITYPROXYMODEL_H
#define COMMODITYPROXYMODEL_H

#include <QSortFilterProxyModel>

#include "../common/enums.h"

class CommodityProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit CommodityProxyModel(QObject *parent = nullptr);

    Q_INVOKABLE void setCommodityType(Enums::CommodityType filterType);

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const Q_DECL_OVERRIDE;

private:
    Enums::CommodityType m_commodityFilterType =  Enums::CommodityType::Timber;
};

#endif // COMMODITYPROXYMODEL_H
