#include "datamanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDateTime>
#include <QVariant>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "../common/enums.h"
#include "../common/globals.h"
#include "../common/location.h"
#include "../common/tags.h"
#include "../helpers/utility.h"
#include "../common/logs.h"

#include <QLoggingCategory>
Q_LOGGING_CATEGORY(datamanager, "datamanager")

DataManager::DataManager(QObject *parent)
    : AbstractManager(parent)
{
    setupModels();
}

void DataManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("dataManager"), this);
    engine.rootContext()->setContextProperty(QStringLiteral("calendarRangeProxyModel"), &m_calendarRangeProxyModel);
    engine.rootContext()->setContextProperty(QStringLiteral("dateEventsRangeProxyModel"), &m_dateEventsRangeProxyModel);
    engine.rootContext()->setContextProperty(QStringLiteral("shipmentEventsProxyModel"), &m_shipmentEventsProxyModel);
    engine.rootContext()->setContextProperty(QStringLiteral("latestEventsProxyModel"), &m_latestEventsProxyModel);
}

void DataManager::setupModels()
{
    m_calendarRangeProxyModel.setSourceModel(&m_eventModel);

    m_dateEventsRangeProxyModel.setSourceModel(&m_eventModel);
    m_dateEventsRangeProxyModel.setSortingTypeAndRole(EventModel::Timestamp, 0, Qt::DescendingOrder);

    m_shipmentEventsProxyModel.setSourceModel(&m_eventModel);
    m_latestEventsProxyModel.setSourceModel(&m_eventModel);
}

QJsonValue DataManager::checkAndValue(const QJsonObject &object, const QLatin1String tag)
{
    if (!object.contains(tag)) {
        qCWarning(datamanager) << "Tag" << tag << "is missing in an object data!";
        return {};
    }
    return object.value(tag);
}

void DataManager::onEntityLoaded(const QJsonObject &entity)
{
    Gaia::ModelData eventData;

    const auto shipmentId = checkAndValue(entity, Tags::id).toString();
    const auto historyArray = checkAndValue(entity, Tags::history).toArray();

    QJsonArray::const_iterator it = historyArray.constBegin();
    while (it != historyArray.constEnd()) {
        const auto &historyObj = (*it).toObject();

        const auto agentObj = checkAndValue(historyObj, Tags::agent).toObject();
        const auto companyName = checkAndValue(agentObj, Tags::companyName).toString();
        const auto agentRole = Utility::instance()->userTypeFromString(
                    checkAndValue(agentObj, Tags::role).toString());

        const auto date = QDateTime::fromSecsSinceEpoch(
                    checkAndValue(historyObj, Tags::timestamp).toVariant().value<qint64>());
        const auto action = Utility::instance()->supplyChainActionFromString(
                    checkAndValue(historyObj, Tags::action).toString());
        const auto actionProgress = Utility::instance()->actionProgressFromString(
                    checkAndValue(historyObj, Tags::actionProgress).toString());

        const auto locationArray = checkAndValue(historyObj, Tags::location).toArray();
        const auto location = Location{ locationArray.at(0).toDouble(), locationArray.at(1).toDouble() };

        eventData.append(QVariantList({ shipmentId,
                                        date,
                                        QVariant::fromValue(location),
                                        companyName,
                                        QVariant::fromValue(agentRole),
                                        QVariant::fromValue(action),
                                        QVariant::fromValue(actionProgress)
                                      } ));
        ++it;
    }

    Gaia::ModelData shipmentData;
    shipmentData.append({shipmentId, });
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
