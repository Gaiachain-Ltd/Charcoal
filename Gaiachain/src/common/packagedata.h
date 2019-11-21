#ifndef PACKAGEDATA_H
#define PACKAGEDATA_H

#include <QObject>
#include <QHash>
#include <QVariantHash>
#include <QDateTime>

#include "enums.h"

struct PackageData
{
    Q_GADGET
    Q_PROPERTY(QString id MEMBER id)
    Q_PROPERTY(Enums::PackageType type MEMBER type)
    Q_PROPERTY(QStringList relatedPackages MEMBER relatedPackages)
    Q_PROPERTY(QVariantMap properties MEMBER properties)

    // property names
    Q_PROPERTY(QString ProducerId MEMBER ProducerId CONSTANT)
    Q_PROPERTY(QString ProducerName MEMBER ProducerName CONSTANT)
    Q_PROPERTY(QString Village MEMBER Village CONSTANT)
    Q_PROPERTY(QString ParcelCode MEMBER ParcelCode CONSTANT)
    Q_PROPERTY(QString BreakingDate MEMBER BreakingDate CONSTANT)
    Q_PROPERTY(QString DryingDate MEMBER DryingDate CONSTANT)
    Q_PROPERTY(QString EstimatedVolume MEMBER EstimatedVolume CONSTANT)
    Q_PROPERTY(QString DepartureDate MEMBER DepartureDate CONSTANT)
    Q_PROPERTY(QString DeparturePlace MEMBER DeparturePlace CONSTANT)
    Q_PROPERTY(QString Buyer MEMBER Buyer CONSTANT)
    Q_PROPERTY(QString Weights MEMBER Weights CONSTANT)
    Q_PROPERTY(QString Transporter MEMBER Transporter CONSTANT)
    Q_PROPERTY(QString Destination MEMBER Destination CONSTANT)
    Q_PROPERTY(QString Weight MEMBER Weight CONSTANT)

public:
    Q_INVOKABLE QVariantList actions() const;
    Q_INVOKABLE QDateTime actionDate(const Enums::SupplyChainAction &action) const;

    QString id;
    Enums::PackageType type;
    QStringList relatedPackages;

    QHash<Enums::SupplyChainAction, QDateTime> actionDates;
    QVariantMap properties;

    // property names
    static const QString ProducerId;
    static const QString ProducerName;
    static const QString Village;
    static const QString ParcelCode;
    static const QString BreakingDate;
    static const QString DryingDate;
    static const QString EstimatedVolume;
    static const QString DepartureDate;
    static const QString DeparturePlace;
    static const QString Buyer;
    static const QString Weights;
    static const QString Transporter;
    static const QString Destination;
    static const QString Weight;
};
Q_DECLARE_METATYPE(PackageData)

#endif // PACKAGEDATA_H
