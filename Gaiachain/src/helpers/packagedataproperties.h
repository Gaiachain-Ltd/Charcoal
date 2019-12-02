#ifndef PACKAGEDATAPROPERTIES_H
#define PACKAGEDATAPROPERTIES_H

#include <QObject>

class PackageDataProperties : public QObject {
    Q_OBJECT
private:
    // property names
    Q_PROPERTY(QString ProducerId MEMBER ProducerId CONSTANT)
    Q_PROPERTY(QString ProducerName MEMBER ProducerName CONSTANT)
    Q_PROPERTY(QString Village MEMBER Village CONSTANT)
    Q_PROPERTY(QString ParcelCode MEMBER ParcelCode CONSTANT)
    Q_PROPERTY(QString BreakingDate MEMBER BreakingDate CONSTANT)
    Q_PROPERTY(QString DryingDate MEMBER DryingDate CONSTANT)
    Q_PROPERTY(QString EstimatedVolume MEMBER EstimatedVolume CONSTANT)
    Q_PROPERTY(QString DepartureDate MEMBER DepartureDate CONSTANT)
    Q_PROPERTY(QString HarvestDate MEMBER HarvestDate CONSTANT)
    Q_PROPERTY(QString DeparturePlace MEMBER DeparturePlace CONSTANT)
    Q_PROPERTY(QString Buyer MEMBER Buyer CONSTANT)
    Q_PROPERTY(QString Weights MEMBER Weights CONSTANT)
    Q_PROPERTY(QString Transporter MEMBER Transporter CONSTANT)
    Q_PROPERTY(QString Destination MEMBER Destination CONSTANT)
    Q_PROPERTY(QString Weight MEMBER Weight CONSTANT)
    Q_PROPERTY(QString TransportDate MEMBER TransportDate CONSTANT)

public:
    static PackageDataProperties &instance();

    // property names
    static const QString ProducerId;
    static const QString ProducerName;
    static const QString Village;
    static const QString ParcelCode;
    static const QString BreakingDate;
    static const QString DryingDate;
    static const QString EstimatedVolume;
    static const QString DepartureDate;
    static const QString HarvestDate;
    static const QString DeparturePlace;
    static const QString Buyer;
    static const QString Weights;
    static const QString Transporter;
    static const QString Destination;
    static const QString Weight;
    static const QString TransportDate;

private:
    PackageDataProperties(QObject *parent=nullptr);


};

#endif // PACKAGEDATAPROPERTIES_H
