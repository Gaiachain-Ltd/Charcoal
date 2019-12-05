#include "packagedataproperties.h"

PackageDataProperties::PackageDataProperties(QObject *parent)
    : QObject(parent) {

}

const QString PackageDataProperties::ParcelId = QStringLiteral("parcel");
const QString PackageDataProperties::BreakingDate = QStringLiteral("breaking_date");
const QString PackageDataProperties::FermentationEndDate = QStringLiteral("end_fermentation_date");
const QString PackageDataProperties::EstimatedBeansVolume = QStringLiteral("beans_volume");
const QString PackageDataProperties::DepartureDate = QStringLiteral("departure_date");
const QString PackageDataProperties::HarvestDate = QStringLiteral("harvest_date");
const QString PackageDataProperties::DeparturePlace = QStringLiteral("departure_place");
const QString PackageDataProperties::Buyer = QStringLiteral("buyer");
const QString PackageDataProperties::Weights = QStringLiteral("weights");
const QString PackageDataProperties::Transporter = QStringLiteral("transporter");
const QString PackageDataProperties::Destination = QStringLiteral("destination");
const QString PackageDataProperties::Weight = QStringLiteral("weight");
const QString PackageDataProperties::ReceptionDate = QStringLiteral("reception_date");
const QString PackageDataProperties::TransportDate = QStringLiteral("transport_date");

PackageDataProperties &PackageDataProperties::instance() {
    static PackageDataProperties pdp;
    return pdp;
}
