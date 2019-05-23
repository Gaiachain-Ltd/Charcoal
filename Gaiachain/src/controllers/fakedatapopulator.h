#ifndef FAKEDATAPOPULATOR_H
#define FAKEDATAPOPULATOR_H

#include <QObject>
#include <QHash>

#include "../common/enums.h"

class EventModel;
class ShipmentModel;

class FakeDataPopulator
{
public:
    explicit FakeDataPopulator(EventModel &eventModel, ShipmentModel &shipmentModel);

    void populateFakeData(const int count, const Enums::CommodityType commodityType);

private:
    QStringList generateRandomIdList(const int count) const;
    QString generateRandomId() const;

    EventModel &m_eventModel;
    ShipmentModel &m_shipmentModel;

    const QHash<Enums::UserType, QStringList> m_companyNames {
        { Enums::UserType::Producer, {"ForestryInc", "Fortrest", "Fory"} },
        { Enums::UserType::LogParkWorker, {"LogParkInc", "Park", "LoggingInc"} },
        { Enums::UserType::SawmillWorker, {"SawmilWorkersInc", "Saws", "Sawmill"} },
        { Enums::UserType::Exporter, {"ExportingInc", "ShipsInc", "XentExports"} },
        { Enums::UserType::VillageWorker, {"VillageInc", "VillagesAndBrothers"} },
        { Enums::UserType::BaggingWorker, {"BaggingInc", "BagsAndRoses", "RisingBags"} },
        { Enums::UserType::NurseryWorker, {"NursyInc", "NuresersRos"} },
        { Enums::UserType::TruckDriver, {"TruckersGoing", "TruckersInc"} }
    };
};

#endif // FAKEDATAPOPULATOR_H
