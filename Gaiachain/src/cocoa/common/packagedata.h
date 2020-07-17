#ifndef PACKAGEDATA_H
#define PACKAGEDATA_H

#include <QObject>
#include <QHash>
#include <QVariantHash>
#include <QDateTime>

#include "common/enums.h"

struct PackageData
{
    Q_GADGET
    Q_PROPERTY(QString id MEMBER id)
    Q_PROPERTY(Enums::PackageType type MEMBER type)
    Q_PROPERTY(quint32 cooperativeId MEMBER cooperativeId)
    Q_PROPERTY(QStringList relatedPackages MEMBER relatedPackages)
    Q_PROPERTY(QVariantMap properties MEMBER properties)
public:
    Q_INVOKABLE QVariantList actions() const;
    Q_INVOKABLE QDateTime actionDate(const Enums::SupplyChainAction &action) const;

    QString id;
    Enums::PackageType type;

    quint32 cooperativeId;

    QStringList relatedPackages;

    QHash<Enums::SupplyChainAction, QDateTime> actionDates;
    QVariantMap properties;
};
Q_DECLARE_METATYPE(PackageData)

#endif // PACKAGEDATA_H
