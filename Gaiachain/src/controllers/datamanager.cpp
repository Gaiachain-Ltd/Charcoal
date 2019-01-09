#include "datamanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDateTime>

#include "../common/globals.h"

DataManager::DataManager(QObject *parent)
    : AbstractManager(parent)
{
    setupModels();
    populateModels();
}

void DataManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("commodityRangeProxy"), &m_commodityDateRangeProxyModel);
    engine.rootContext()->setContextProperty(QStringLiteral("commodityProxy"), &m_commodityProxyModel);
}

void DataManager::setupModels()
{
    m_commodityProxyModel.setSourceModel(&m_shipmentModel);

    m_commodityDateRangeProxyModel.setSourceModel(&m_eventModel);
    m_commodityDateRangeProxyModel.setCommodityProxyModel(&m_commodityProxyModel);
}

#include <QDebug>
void DataManager::populateModels()
{
    Gaia::ModelData shipmentData;
    Gaia::ModelData eventData;

    // Populate shipment data
    int ctCount = static_cast<int>(Enums::CommodityType::CommodityCount);
    int shipmentCount = 100;
    for (int i = 0; i < shipmentCount; ++i) {
        shipmentData.append({i, i%ctCount});
    }
    m_shipmentModel.appendData(shipmentData);

    //Populate events data
    int eventCount = 100;
    QStringList comapnyNames{"Milo", "Solutions", "XentCorp", "YOLOCorp"};
    QDateTime startDate(QDate(2018,1,1));
    QDateTime endDate(QDate(2019,1,1));
    auto dateDiff = startDate.daysTo(endDate);


    QDateTime startTestRange(QDate(2018,12,1));
    QDateTime endTestRange(QDate(2019,1,1));
    int countTest = 0;

    for (int i = 0; i < eventCount; ++i) {
        int shipmentId = rand() % shipmentCount;
        QString companyName = comapnyNames[rand() % comapnyNames.size()];
        int daysElapsed = rand() % dateDiff;

        QDateTime arrivalDateTime = startDate.addDays(daysElapsed);
        QDateTime departureDateTime = arrivalDateTime.addDays(rand() % 15 + 1);

        if (arrivalDateTime >= startTestRange && arrivalDateTime < endTestRange)
            ++countTest;

        if (departureDateTime >= startTestRange && departureDateTime < endTestRange)
            ++countTest;

        eventData.append({i,
                          shipmentId,
                          companyName,
                          shipmentId,
                          arrivalDateTime,
                          departureDateTime
                         });
    }

    qDebug() << "Count between dates:" << startTestRange << endTestRange <<  countTest;

    m_eventModel.appendData(eventData);
}
