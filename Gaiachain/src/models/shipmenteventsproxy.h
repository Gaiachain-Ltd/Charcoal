#ifndef SHIPMENTEVENTSPROXY_H
#define SHIPMENTEVENTSPROXY_H

#include "abstractsortfilterproxymodel.h"

class ShipmentEventsProxy : public AbstractSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ShipmentEventsProxy(QObject *parent = nullptr);

    Q_INVOKABLE void clearShipmentId();
    Q_INVOKABLE void setShipmentId(const QString &shipmentId);

    Q_INVOKABLE QVariantMap getRowAttributes(int row);

signals:
    void shipmentIdChanged() const;

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QString m_shipmentId;
};

#endif // SHIPMENTEVENTSPROXY_H
