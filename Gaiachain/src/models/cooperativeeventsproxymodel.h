#ifndef COOPERATIVEEVENTSPROXYMODEL_H
#define COOPERATIVEEVENTSPROXYMODEL_H

#include "abstractsortfilterproxymodel.h"

class CooperativeEventsProxyModel : public AbstractSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(quint32 cooperativeId READ cooperativeId WRITE setCooperativeId NOTIFY cooperativeIdChanged)
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)

public:
    explicit CooperativeEventsProxyModel(QObject *parent = nullptr);

    quint32 cooperativeId() const;
    bool active() const;

public slots:
    void setCooperativeId(quint32 cooperativeId);
    void setActive(bool active);

signals:
    void cooperativeIdChanged(quint32 cooperativeId) const;
    void activeChanged(bool active) const;

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    quint32 m_cooperativeId = 0;
    bool m_active = true;
};

#endif // COOPERATIVEEVENTSPROXYMODEL_H
