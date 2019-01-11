#include "datamanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDateTime>

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
    engine.rootContext()->setContextProperty(QStringLiteral("commodityRangeProxy"), &m_commodityDateRangeProxyModel);
    engine.rootContext()->setContextProperty(QStringLiteral("eventsDateRangeProxyModel"), &m_eventsDateRangeProxyModel);
}

void DataManager::setupModels()
{
    m_commodityProxyModel.setSourceModel(&m_shipmentModel);

    m_commodityDateRangeProxyModel.setSourceModel(&m_eventModel);
    m_commodityDateRangeProxyModel.setCommodityProxyModel(&m_commodityProxyModel);

    m_eventsDateRangeProxyModel.setSourceModel(&m_eventModel);
    m_eventsDateRangeProxyModel.setCommodityProxyModel(&m_commodityProxyModel);
}

#include <QDebug>
void DataManager::populateModels()
{
    Gaia::ModelData shipmentData;
    Gaia::ModelData eventData;

    // Populate shipment data
    int ctCount = static_cast<int>(Enums::CommodityType::CommodityCount);
    int shipmentCount = 300;
    for (int i = 0; i < shipmentCount; ++i) {
        shipmentData.append({QString::number(i), i % ctCount});
    }
    m_shipmentModel.appendData(shipmentData);

    //Populate events data
    QStringList comapnyNames{"Milo", "Solutions", "XentCorp", "YOLOCorp"};
    QVector<Enums::PlaceType> placeType{Enums::PlaceType::Forestery, Enums::PlaceType::LogPark,
                Enums::PlaceType::Sawmill, Enums::PlaceType::Export};
    QVector<Enums::PlaceAction> actions {Enums::PlaceAction::Arrived, Enums::PlaceAction::Departed};

    QDateTime startDate(QDate(2018,1,1));
    QDateTime endDate(QDate(2019,1,1));
    auto dateDiff = startDate.daysTo(endDate) / 4;

    QDateTime startTestRange = startDate;//(QDate(2018,12,1));
    QDateTime endTestRange = endDate;//(QDate(2019,1,1));
    int countTest = 0;

    for (int i =0; i < shipmentCount; ++i) {
        int shipId = i;
        QDateTime sd = startDate;

        for (const auto &place : placeType) {
            for (const auto &action : actions) {
                QString companyName = comapnyNames[rand() % comapnyNames.size()];

                int daysElapsed = (rand() % dateDiff + 1);
                sd = sd.addDays(daysElapsed);

                if (sd >= startTestRange || sd <= endTestRange)
                    ++countTest;

                Location loc;
                loc.lat = 9.665645 + i;
                loc.lon = 7.324432 + i;

                eventData.append({shipId,
                                  sd,
                                  QVariant::fromValue(loc),
                                  companyName,
                                  static_cast<int>(place),
                                  static_cast<int>(action)
                                 });
            }
        }
    }

    qDebug() << "Count between dates:" << startTestRange << endTestRange <<  countTest;

    m_eventModel.appendData(eventData);
}
