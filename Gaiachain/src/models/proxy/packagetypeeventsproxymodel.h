#ifndef PACKAGETYPEEVENTSPROXYMODEL_H
#define PACKAGETYPEEVENTSPROXYMODEL_H

#include <QDateTime>
#include <QDate>
#include <QSet>

#include "abstractidentityproxymodel.h"
#include "../../common/globals.h"
#include "../../common/enums.h"

class PackageTypeEventsProxyModel : public AbstractIdentityProxyModel
{
    Q_OBJECT

public:
    explicit PackageTypeEventsProxyModel(QObject *parent = nullptr);

    QHash<Enums::PackageType, int> packageTypeEvents() const;
    QList<Enums::PackageType> datePackageTypes(const QDate &date) const;

    Q_INVOKABLE QVariantMap packageTypeEventsQml() const;
    Q_INVOKABLE QVariantList datePackageTypesQml(const QDate &date) const;

signals:
    void packageTypeEventsChanged(const Enums::PackageType &packageType) const;
    void datePackageTypesChanged(const QDate &date) const;

private:
    static const QHash<int, QByteArray> sc_roleNames;

    QHash<Enums::PackageType, int> m_packageTypeEvents;
    QHash<QDate, QHash<Enums::PackageType, int> > m_datesPackageTypeEvents;

private slots:
    void onRowsInserted(const QModelIndex &parent, int first, int last);
    void onRowsToBeRemoved(const QModelIndex &parent, int first, int last);
    void onModelReset();
};

#endif // PACKAGETYPEEVENTSPROXYMODEL_H
