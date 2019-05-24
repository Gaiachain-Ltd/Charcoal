#include "datamanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "../common/enums.h"
#include "../common/globals.h"
#include "../common/location.h"
#include "../common/tags.h"
#include "../helpers/utility.h"
#include "../common/logs.h"

DataManager::DataManager(QObject *parent)
    : AbstractManager(parent)
#ifdef FAKE_DATA
    , m_fakeDataPopulator(m_eventModel, m_shipmentModel)
#endif
{
    setupModels();
#ifdef FAKE_DATA
    m_fakeDataPopulator.populateFakeData(20, Enums::CommodityType::Timber);
    m_fakeDataPopulator.populateFakeData(20, Enums::CommodityType::Cocoa);
#endif
}

void DataManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("dataManager"), this);
    engine.rootContext()->setContextProperty(QStringLiteral("commodityProxyModel"), &m_commodityProxyModel);
    engine.rootContext()->setContextProperty(QStringLiteral("calendarRangeProxyModel"), &m_calendarRangeProxyModel);
    engine.rootContext()->setContextProperty(QStringLiteral("dateEventsRangeProxyModel"), &m_dateEventsRangeProxyModel);
    engine.rootContext()->setContextProperty(QStringLiteral("shipmentEventsProxyModel"), &m_shipmentEventsProxyModel);
    engine.rootContext()->setContextProperty(QStringLiteral("latestEventsProxyModel"), &m_latestEventsProxyModel);
#ifdef FAKE_DATA
    engine.rootContext()->setContextProperty(QStringLiteral("fakeDataPopulator"), &m_fakeDataPopulator);
#endif
}

void DataManager::setupModels()
{
    m_commodityProxyModel.setSourceModel(&m_shipmentModel);

    m_calendarRangeProxyModel.setSourceModel(&m_eventModel);
    m_calendarRangeProxyModel.setCommodityProxyModel(&m_commodityProxyModel);

    m_dateEventsRangeProxyModel.setSourceModel(&m_eventModel);
    m_dateEventsRangeProxyModel.setSortingTypeAndRole(EventModel::Timestamp, 0, Qt::DescendingOrder);
    m_dateEventsRangeProxyModel.setCommodityProxyModel(&m_commodityProxyModel);

    m_shipmentEventsProxyModel.setSourceModel(&m_eventModel);
    m_latestEventsProxyModel.setSourceModel(&m_eventModel);
    m_latestEventsProxyModel.setCommodityProxyModel(&m_commodityProxyModel);
}

void DataManager::onEntityLoaded(const QJsonObject &entity)
{
    Gaia::ModelData eventData;

    const QString shipmentId = entity.value(Tags::id).toString();
    const QJsonArray &history = entity.value(Tags::history).toArray();
    QJsonArray::const_iterator it = history.constBegin();

    while (it != history.constEnd()) {
        const QJsonObject &historyObj = (*it).toObject();
        const QDateTime date = QDateTime::fromSecsSinceEpoch(static_cast<uint>(historyObj.value(Tags::timestamp).toInt(0)));
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
                          QVariant::fromValue(Utility::instance()->userTypeFromString(agentRole)),
                          QVariant::fromValue(placeAction)
                         });
        ++it;
    }

    Gaia::ModelData shipmentData;
    shipmentData.append({shipmentId, QVariant::fromValue(Enums::CommodityType::Timber)});
    m_shipmentModel.appendData(shipmentData);

    m_eventModel.appendData(eventData);
}

void DataManager::onEntitiesLoaded(const QJsonArray &entities)
{
    QJsonArray::const_iterator it = entities.constBegin();
    while (it != entities.constEnd()) {
        const QJsonObject &obj = (*it).toObject();
        onEntityLoaded(obj);
        ++it;
    }
}

void DataManager::clearModels()
{
    m_eventModel.clearModel();
    m_shipmentModel.clearModel();
}
