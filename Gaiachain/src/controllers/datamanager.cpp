#include "datamanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

#include "../common/enums.h"
#include "../common/globals.h"
#include "../common/location.h"
#include "../common/tags.h"

DataManager::DataManager(QObject *parent)
    : AbstractManager(parent)
{
    setupModels();
//    populateModels();
}

void DataManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("commodityProxyModel"), &m_commodityProxyModel);
    engine.rootContext()->setContextProperty(QStringLiteral("calendarRangeProxyModel"), &m_calendarRangeProxyModel);
    engine.rootContext()->setContextProperty(QStringLiteral("dateEventsRangeProxyModel"), &m_dateEventsRangeProxyModel);
    engine.rootContext()->setContextProperty(QStringLiteral("shipmentEventsProxyModel"), &m_shipmentEventsProxyModel);
    engine.rootContext()->setContextProperty(QStringLiteral("latestEventsProxyModel"), &m_latestEventsProxyModel);
}

void DataManager::setupModels()
{
    m_commodityProxyModel.setSourceModel(&m_shipmentModel);

    m_calendarRangeProxyModel.setSourceModel(&m_eventModel);
    m_calendarRangeProxyModel.setCommodityProxyModel(&m_commodityProxyModel);

    m_dateEventsRangeProxyModel.setSourceModel(&m_eventModel);
    m_dateEventsRangeProxyModel.setCommodityProxyModel(&m_commodityProxyModel);

    m_shipmentEventsProxyModel.setSourceModel(&m_eventModel);
    m_latestEventsProxyModel.setSourceModel(&m_eventModel);
}

void DataManager::onEntityLoaded(const QJsonDocument &doc)
{
    Gaia::ModelData eventData;

    const QJsonObject &obj = doc.object();
    const QString shipmentId = obj.value(Tags::id).toString();
    const QJsonArray &history = obj.value(Tags::history).toArray();
    QJsonArray::const_iterator it = history.constBegin();
    while (it != history.constEnd()) {
        const QJsonObject &historyObj = (*it).toObject();
        const QDateTime date = QDateTime::fromTime_t(static_cast<uint>(historyObj.value(Tags::timestamp).toInt(0)));
        const QJsonObject agent = historyObj.value(Tags::agent).toObject();
        const QString companyName = agent.value(Tags::companyName).toString();
        const QString agentRole = agent.value(Tags::role).toString();
        const QString action = historyObj.value(Tags::action).toString();
        Enums::PlaceAction placeAction = Enums::PlaceAction::Arrived;
        // TO_DO handling strings to enum conversions
        if (action == QStringLiteral("DEPARTED"))
            placeAction = Enums::PlaceAction::Departed;
        const QJsonArray locationArray = historyObj.value(Tags::location).toArray();
        Location loc;
        loc.lat = locationArray.at(0).toDouble();
        loc.lon = locationArray.at(1).toDouble();

        eventData.append({shipmentId,
                          date,
                          QVariant::fromValue(loc),
                          companyName,
                          static_cast<int>(Enums::UserTypeStruct::userTypeFromString(agentRole)),
                          QVariant::fromValue(placeAction)
                         });
        ++it;
    }

    Gaia::ModelData shipmentData;
    shipmentData.append({shipmentId, QVariant::fromValue(Enums::CommodityType::Timber)});
    m_shipmentModel.appendData(shipmentData);

    m_eventModel.appendData(eventData);
}

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
