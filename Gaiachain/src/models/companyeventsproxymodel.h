#ifndef COMPANYEVENTSPROXYMODEL_H
#define COMPANYEVENTSPROXYMODEL_H

#include "abstractsortfilterproxymodel.h"

class CompanyEventsProxyModel : public AbstractSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)

public:
    explicit CompanyEventsProxyModel(QObject *parent = nullptr);

    void setCompanyId(const QString &companyId);

    bool active() const;

public slots:
    void setActive(bool active);

signals:
    void activeChanged(bool active);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    bool m_active = false;
};

#endif // COMPANYEVENTSPROXYMODEL_H
