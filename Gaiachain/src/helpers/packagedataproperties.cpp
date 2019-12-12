#include "packagedataproperties.h"

PackageDataProperties::PackageDataProperties(QObject *parent)
    : QObject(parent)
{}

const QString PackageDataProperties::ParcelId = QStringLiteral("parcel");
const QString PackageDataProperties::HarvestDate = QStringLiteral("harvest_date");
const QString PackageDataProperties::BreakingDate = QStringLiteral("breaking_date");
const QString PackageDataProperties::FermentationEndDate = QStringLiteral("end_fermentation_date");
const QString PackageDataProperties::EstimatedBeansVolume = QStringLiteral("beans_volume");
const QString PackageDataProperties::ReceptionDate = QStringLiteral("reception_date");
const QString PackageDataProperties::TransportDate = QStringLiteral("transport_date");
const QString PackageDataProperties::Buyer = QStringLiteral("buyer");
const QString PackageDataProperties::LotPid = QStringLiteral("lot_pid");
const QString PackageDataProperties::HarvestWeights = QStringLiteral("harvest_weights");
const QString PackageDataProperties::Pid = QStringLiteral("pid");
const QString PackageDataProperties::Weight = QStringLiteral("weight");
const QString PackageDataProperties::Notes = QStringLiteral("notes");
const QString PackageDataProperties::TransporterId = QStringLiteral("transporter");
const QString PackageDataProperties::DestinationId = QStringLiteral("destination");

PackageDataProperties &PackageDataProperties::instance() {
    static PackageDataProperties pdp;
    return pdp;
}
