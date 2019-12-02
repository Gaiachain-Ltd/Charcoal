#include "packagedataproperties.h"

PackageDataProperties::PackageDataProperties(QObject *parent)
    : QObject(parent) {

}

const QString PackageDataProperties::ProducerId = QStringLiteral("producerId");
const QString PackageDataProperties::ProducerName = QStringLiteral("producerName");
const QString PackageDataProperties::Village = QStringLiteral("village");
const QString PackageDataProperties::ParcelCode = QStringLiteral("parcelCode");
const QString PackageDataProperties::BreakingDate = QStringLiteral("breakingDate");
const QString PackageDataProperties::DryingDate = QStringLiteral("dryingDate");
const QString PackageDataProperties::EstimatedVolume = QStringLiteral("estimatedVolume");
const QString PackageDataProperties::DepartureDate = QStringLiteral("departureDate");
const QString PackageDataProperties::HarvestDate = QStringLiteral("harvestDate");
const QString PackageDataProperties::DeparturePlace = QStringLiteral("departurePlace");
const QString PackageDataProperties::Buyer = QStringLiteral("buyer");
const QString PackageDataProperties::Weights = QStringLiteral("weights");
const QString PackageDataProperties::Transporter = QStringLiteral("transporter");
const QString PackageDataProperties::Destination = QStringLiteral("destination");
const QString PackageDataProperties::Weight = QStringLiteral("weight");
const QString PackageDataProperties::TransportDate = QStringLiteral("transportDate");

PackageDataProperties &PackageDataProperties::instance() {
    static PackageDataProperties pdp;
    return pdp;
}
