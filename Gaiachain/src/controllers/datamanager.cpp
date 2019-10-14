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
#include "../common/dataglobals.h"
#include "../common/logs.h"

#include <QLoggingCategory>
Q_LOGGING_CATEGORY(dataManager, "data.manager")

DataManager::DataManager(QObject *parent)
    : AbstractManager(parent)
{
    setupModels();
}

void DataManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("dataManager"), this);
    engine.rootContext()->setContextProperty(QStringLiteral("calendarModel"), &m_calendarModel);
    engine.rootContext()->setContextProperty(QStringLiteral("calendarPackageTypesModel"), &m_calendarPackagesTypesModel);
    engine.rootContext()->setContextProperty(QStringLiteral("dateEventsModel"), &m_dateEventsModel);
    engine.rootContext()->setContextProperty(QStringLiteral("latestDateEventsModel"), &m_latestDateEventsModel);
    engine.rootContext()->setContextProperty(QStringLiteral("shipmentEventsModel"), &m_shipmentEventsModel);
    engine.rootContext()->setContextProperty(QStringLiteral("latestEventsModel"), &m_latestEventsModel);
}

void DataManager::setupModels()
{
    m_calendarModel.setSourceModel(&m_eventModel);
    m_calendarPackagesTypesModel.setSourceModel(&m_calendarModel);

    m_dateEventsModel.setSourceModel(&m_eventModel);
    m_latestDateEventsModel.setSourceModel(&m_dateEventsModel);

    m_shipmentEventsModel.setSourceModel(&m_eventModel);
    m_latestEventsModel.setSourceModel(&m_eventModel);
}

QJsonValue DataManager::checkAndValue(const QJsonObject &object, const QLatin1String tag)
{
    if (!object.contains(tag)) {
        qCWarning(dataManager) << "Tag" << tag << "is missing in an object data!";
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
        const auto agentRole = DataGlobals::userTypeFromString(
                    checkAndValue(agentObj, Tags::role).toString());

        const auto date = QDateTime::fromSecsSinceEpoch(
                    checkAndValue(historyObj, Tags::timestamp).toVariant().value<qint64>());
        const auto action = DataGlobals::supplyChainActionFromString(
                    checkAndValue(historyObj, Tags::action).toString());
        const auto actionProgress = DataGlobals::actionProgressFromString(
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

    m_shipmentEventsModel.setShipmentId(entities.first().toObject().value(Tags::id).toString());
}

void DataManager::clearModels()
{
    m_eventModel.clearModel();
}
