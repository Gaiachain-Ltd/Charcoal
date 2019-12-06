#ifndef PACKAGEDATAPROPERTIES_H
#define PACKAGEDATAPROPERTIES_H

#include <QObject>

class PackageDataProperties : public QObject {
    Q_OBJECT
private:
    // property names
    Q_PROPERTY(QString ParcelId MEMBER ParcelId CONSTANT)
    Q_PROPERTY(QString HarvestDate MEMBER HarvestDate CONSTANT)
    Q_PROPERTY(QString BreakingDate MEMBER BreakingDate CONSTANT)
    Q_PROPERTY(QString FermentationEndDate MEMBER FermentationEndDate CONSTANT)
    Q_PROPERTY(QString EstimatedBeansVolume MEMBER EstimatedBeansVolume CONSTANT)
    Q_PROPERTY(QString ReceptionDate MEMBER ReceptionDate CONSTANT)
    Q_PROPERTY(QString TransportDate MEMBER TransportDate CONSTANT)
    Q_PROPERTY(QString Buyer MEMBER Buyer CONSTANT)
    Q_PROPERTY(QString LotPid MEMBER LotPid CONSTANT)
    Q_PROPERTY(QString HarvestWeights MEMBER HarvestWeights CONSTANT)
    Q_PROPERTY(QString Pid MEMBER Pid CONSTANT)
    Q_PROPERTY(QString Weight MEMBER Weight CONSTANT)
    Q_PROPERTY(QString Notes MEMBER Notes CONSTANT)
    Q_PROPERTY(QString TransporterId MEMBER TransporterId CONSTANT)
    Q_PROPERTY(QString DestinationId MEMBER DestinationId CONSTANT)

public:
    static PackageDataProperties &instance();

    // property names
    static const QString ParcelId;
    static const QString HarvestDate;
    static const QString BreakingDate;
    static const QString FermentationEndDate;
    static const QString EstimatedBeansVolume;
    static const QString ReceptionDate;
    static const QString TransportDate;
    static const QString Buyer;
    static const QString LotPid;
    static const QString HarvestWeights;
    static const QString Pid;
    static const QString Weight;
    static const QString Notes;
    static const QString TransporterId;
    static const QString DestinationId;

private:
    PackageDataProperties(QObject *parent = nullptr);


};

#endif // PACKAGEDATAPROPERTIES_H
