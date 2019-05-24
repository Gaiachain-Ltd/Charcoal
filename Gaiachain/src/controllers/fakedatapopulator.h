#ifndef FAKEDATAPOPULATOR_H
#define FAKEDATAPOPULATOR_H

#include <QObject>
#include <QHash>
#include <QVector>
#include <QVariantMap>

#include "../common/enums.h"

class EventModel;
class ShipmentModel;

class FakeDataPopulator : public QObject
{
    Q_OBJECT
public:
    explicit FakeDataPopulator(EventModel &eventModel, ShipmentModel &shipmentModel);

    void populateFakeData(const int count, const Enums::CommodityType commodityType);

    Q_INVOKABLE void addId(const QVariantMap &attributes, const Enums::CommodityType commodityType);
    Q_INVOKABLE bool canAddId(const QString &id, const Enums::CommodityType commodityType) const;
    Q_INVOKABLE QVariantMap getIdNextData(const QString &id, const Enums::CommodityType commodityType) const;

private:
    QStringList generateRandomIdList(const int count) const;
    QString generateRandomId() const;
    Enums::PlaceType getNextPlaceType(const Enums::PlaceType place, const Enums::CommodityType commodityType) const;

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

    const QVector<Enums::PlaceType> m_timberPlaceList {
        Enums::PlaceType::Forestery,
        Enums::PlaceType::LogPark,
        Enums::PlaceType::Sawmill,
        Enums::PlaceType::Export,
    };
    const QVector<Enums::PlaceType> m_cocoaPlaceList {
        Enums::PlaceType::Nursery,
        Enums::PlaceType::Village,
        Enums::PlaceType::Bagging,
        Enums::PlaceType::Truck,
        Enums::PlaceType::Export
    };
};

#endif // FAKEDATAPOPULATOR_H
