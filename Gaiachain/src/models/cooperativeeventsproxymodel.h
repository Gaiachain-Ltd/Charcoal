#ifndef COOPERATIVEEVENTSPROXYMODEL_H
#define COOPERATIVEEVENTSPROXYMODEL_H

#include "abstractsortfilterproxymodel.h"

class CooperativeEventsProxyModel : public AbstractSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)

public:
    explicit CooperativeEventsProxyModel(QObject *parent = nullptr);

    void setCooperativeId(const QString &cooperativeId);

    bool active() const;

public slots:
    void setActive(bool active);

signals:
    void activeChanged(bool active);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    bool m_active = true;
};

#endif // COOPERATIVEEVENTSPROXYMODEL_H
