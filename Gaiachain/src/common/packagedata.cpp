#include "packagedata.h"

#include "../helpers/utility.h"

const QString PackageData::ProducerId = QStringLiteral("producerId");
const QString PackageData::ProducerName = QStringLiteral("producerName");
const QString PackageData::Village = QStringLiteral("village");
const QString PackageData::ParcelCode = QStringLiteral("parcelCode");
const QString PackageData::BreakingDate = QStringLiteral("breakingDate");
const QString PackageData::DryingDate = QStringLiteral("dryingDate");
const QString PackageData::EstimatedVolume = QStringLiteral("estimatedVolume");
const QString PackageData::DepartureDate = QStringLiteral("departureDate");
const QString PackageData::DeparturePlace = QStringLiteral("departurePlace");
const QString PackageData::Buyer = QStringLiteral("buyer");
const QString PackageData::Weights = QStringLiteral("weights");
const QString PackageData::Transporter = QStringLiteral("transporter");
const QString PackageData::Destination = QStringLiteral("destination");
const QString PackageData::Weight = QStringLiteral("weight");

QVariantList PackageData::actions() const
{
    return Utility::toVariantList(actionDates.keys(), QMetaType::Int);
}

QDateTime PackageData::actionDate(const Enums::SupplyChainAction &action) const
{
    return actionDates.value(action);
}
