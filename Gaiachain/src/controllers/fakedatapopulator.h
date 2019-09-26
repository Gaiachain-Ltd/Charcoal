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

    void populateFakeData(const int count);

    Q_INVOKABLE void addId(const QVariantMap &attributes);
    Q_INVOKABLE bool canAddId(const QString &id) const;
    Q_INVOKABLE QVariantMap getIdNextData(const QString &id) const;

private:
    QStringList generateRandomIdList(const int count) const;
    QString generateRandomId() const;
    Enums::PlaceType getNextPlaceType(const Enums::PlaceType place) const;

    EventModel &m_eventModel;
    ShipmentModel &m_shipmentModel;

    const QHash<Enums::UserType, QStringList> m_companyNames {
        { Enums::UserType::Producer, {"VillageInc", "VillagesAndBrothers"} },
        { Enums::UserType::BaggingPerson, {"BaggingInc", "BagsAndRoses", "RisingBags"} },
        { Enums::UserType::Storekeeper, {"StoreInc", "StoringAll"} },
        { Enums::UserType::Exporter, {"ExportingInc", "ShipsInc", "XentExports"} }
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
