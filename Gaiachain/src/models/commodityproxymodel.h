#ifndef COMMODITYPROXYMODEL_H
#define COMMODITYPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QSet>
#include <QHash>

#include "../common/enums.h"

class CommodityProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit CommodityProxyModel(QObject *parent = nullptr);

    Q_INVOKABLE void setCommodityType(Enums::CommodityType filterType, bool enable = true);
    Q_INVOKABLE bool commodityEnabled(Enums::CommodityType filterType) const;

    bool isIdIn(const QString& id) const;

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const Q_DECL_OVERRIDE;

signals:
    void commodityTypeChanged() const;
    void filteringFinished() const;

private:
    QSet<Enums::CommodityType> m_enabledCommodites;
    mutable QSet<QString> m_idx;
};

#endif // COMMODITYPROXYMODEL_H
