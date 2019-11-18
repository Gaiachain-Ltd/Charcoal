#ifndef SELECTEDIDSPROXYMODEL_H
#define SELECTEDIDSPROXYMODEL_H

#include "abstractsortfilterproxymodel.h"

class SelectedIdsProxyModel : public AbstractSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SelectedIdsProxyModel(QObject *parent = nullptr);

    Q_INVOKABLE void setPackageIds(const QStringList &packageIds);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QStringList m_packageIds;
};

#endif // SELECTEDIDSPROXYMODEL_H
