#include "datamanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDateTime>
#include <QDebug>

#include "../common/enums.h"
#include "../common/globals.h"
#include "../common/location.h"

DataManager::DataManager(QObject *parent)
    : AbstractManager(parent)
{
    setupModels();
    populateModels();
}

void DataManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("commodityProxy"), &m_commodityProxyModel);
    engine.rootContext()->setContextProperty(QStringLiteral("calendarRangeProxyModel"), &m_calendarRangeProxyModel);
    engine.rootContext()->setContextProperty(QStringLiteral("dateEventsRangeProxyModel"), &m_dateEventsRangeProxyModel);
    engine.rootContext()->setContextProperty(QStringLiteral("shipmentEventsProxyModel"), &m_shipmentEventsProxyModel);
}

void DataManager::setupModels()
{
    m_commodityProxyModel.setSourceModel(&m_shipmentModel);

    m_calendarRangeProxyModel.setSourceModel(&m_eventModel);
    m_calendarRangeProxyModel.setCommodityProxyModel(&m_commodityProxyModel);

    m_dateEventsRangeProxyModel.setSourceModel(&m_eventModel);
    m_dateEventsRangeProxyModel.setCommodityProxyModel(&m_commodityProxyModel);

    m_shipmentEventsProxyModel.setSourceModel(&m_eventModel);
}

void DataManager::populateModels()
{
    Gaia::ModelData shipmentData;
    Gaia::ModelData eventData;

    // Populate shipment data
    int ctCount = static_cast<int>(Enums::CommodityType::CommodityCount);
    int shipmentCount = 1000;
    for (int i = 0; i < shipmentCount; ++i) {
        shipmentData.append({QString::number(i), i % ctCount});
    }
    m_shipmentModel.appendData(shipmentData);

    //Populate events data
    QStringList comapnyNames{"Milo", "Solutions", "XentCorp", "YOLOCorp"};
    QVector<Enums::PlaceType> placeType{Enums::PlaceType::Forestery, Enums::PlaceType::LogPark,
                Enums::PlaceType::Sawmill, Enums::PlaceType::Export};
    QVector<Enums::PlaceAction> actions {Enums::PlaceAction::Arrived, Enums::PlaceAction::Departed};

    QDateTime startDate(QDate(2015,1,1));
    QDateTime endDate = QDateTime::currentDateTime();
    auto dateDiff = startDate.daysTo(endDate) / 4;

    QDateTime startTestRange = QDateTime(QDate(2019,1,1));
    QDateTime endTestRange = endDate;//(QDate(2019,1,1));
    int countTest = 0;

    for (int i =0; i < shipmentCount; ++i) {
        int shipId = i;
        QDateTime sd = startDate;

        for (const auto &place : placeType) {
            QString companyName = comapnyNames[rand() % comapnyNames.size()];

            for (const auto &action : actions) {
                int daysElapsed = (rand() % dateDiff + 1);
                sd = sd.addDays(daysElapsed);

                if (sd >= startTestRange && sd <= endTestRange)
                    ++countTest;

                Location loc;
                loc.lat = 9.665645 + i;
                loc.lon = 7.324432 + i;

                eventData.append({shipId,
                                  sd,
                                  QVariant::fromValue(loc),
                                  companyName,
                                  QVariant::fromValue(place),
                                  QVariant::fromValue(action)
                                 });
            }
        }
    }

    qDebug() << "Count between dates:" << startTestRange << endTestRange <<  countTest;

    m_eventModel.appendData(eventData);
}
