#include "fakedatapopulator.h"

#include <QDateTime>

#include "../models/eventmodel.h"
#include "../models/shipmentmodel.h"
#include "../common/location.h"
#include "../common/logs.h"

#define ID_LENGTH 12

FakeDataPopulator::FakeDataPopulator(EventModel &eventModel, ShipmentModel &shipmentModel)
    : m_eventModel(eventModel)
    , m_shipmentModel(shipmentModel)
{

}

QString FakeDataPopulator::generateRandomId() const
{
    const QString possibleCharacters("abcdefghijklmnopqrstuvwxyz0123456789");

    QString randomString;
    for(int i = 0; i < ID_LENGTH; ++i) {
        randomString.append(possibleCharacters.at(qrand() % possibleCharacters.length()));
        if (i == 3 || i == 7)
            randomString.append('-');
    }
    return randomString;
}

QStringList FakeDataPopulator::generateRandomIdList(const int count) const
{
    QStringList result;

    for (int i = 0 ; i < count ; i++) {
        QString newId;
        while (newId.isEmpty() || result.contains(newId)) {
            newId = generateRandomId();
        }
        result.push_back(newId);
    }

    return result;
}

void FakeDataPopulator::populateFakeData(const int count, const Enums::CommodityType commodityType)
{
    // TODO
    // scanning
    // map

    QVector<Enums::UserType> userType;

    switch(commodityType) {
    case Enums::CommodityType::Timber:
        userType = {
            Enums::UserType::Producer,
            Enums::UserType::LogParkWorker,
            Enums::UserType::SawmillWorker,
            Enums::UserType::Exporter
        };
        break;
    case Enums::CommodityType::Cocoa:
        userType = {
            Enums::UserType::VillageWorker,
            Enums::UserType::BaggingWorker,
            Enums::UserType::NurseryWorker,
            Enums::UserType::TruckDriver,
            Enums::UserType::Exporter
        };
        break;
    default:
        qFatal("[FAKE_DATA] Not supported commodityType!");
    }

    const QVector<Enums::PlaceAction> actions {
        Enums::PlaceAction::Arrived,
        Enums::PlaceAction::Departed
    };

    const QDateTime startDate(QDate(2019,1,1));
    const QDateTime endDate = QDateTime::currentDateTime();
    const auto dateDiff = startDate.daysTo(endDate) / 4;

    // Timber
    int i = 0;
    for (const QString &shipmentId : generateRandomIdList(count)) {
        ++i;
        Gaia::ModelData eventData;

        QDateTime sd = startDate;

        int stepCount = 0;
        const int maxStepCount = qrand() % (userType.size() * actions.size());
        qDebug() << BLUE("[FAKE_DATA]") << commodityType << shipmentId << (maxStepCount + 1);

        for (const auto &user : userType) {
            const QStringList &companyNameList = m_companyNames[user];
            const QString &companyName = companyNameList[rand() % companyNameList.size()];

            for (const auto &action : actions) {
                int daysElapsed = (rand() % dateDiff + 1);
                sd = sd.addDays(daysElapsed);
                qDebug() << sd;

                eventData.append({shipmentId,
                                  sd,
                                  QVariant::fromValue(Location { 9.665645 + i, 7.324432 + i }),
                                  companyName,
                                  QVariant::fromValue(user),
                                  QVariant::fromValue(action)
                                 });
                ++stepCount;
                if (stepCount > maxStepCount)
                    break;
            }
            if (stepCount > maxStepCount)
                break;
        }

        m_eventModel.appendData(eventData);

        Gaia::ModelData shipmentData;
        shipmentData.append({shipmentId, QVariant::fromValue(commodityType)});
        m_shipmentModel.appendData(shipmentData);
    }
}
