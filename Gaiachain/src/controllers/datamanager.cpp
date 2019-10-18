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
    engine.rootContext()->setContextProperty(QStringLiteral("latestEventsModel"), &m_latestEventsModel);
}

void DataManager::clearModels()
{
    m_eventModel.clearModel();
}

void DataManager::onEntitiesLoaded(const QJsonArray &entities)
{
    QJsonArray::const_iterator it = entities.constBegin();
    while (it != entities.constEnd()) {
        const QJsonObject &obj = (*it).toObject();
        loadEntity(obj);
        ++it;
    }
}

void DataManager::setupModels()
{
    m_calendarModel.setSourceModel(&m_eventModel);
    m_calendarPackagesTypesModel.setSourceModel(&m_calendarModel);

    m_dateEventsModel.setSourceModel(&m_eventModel);
    m_latestDateEventsModel.setSourceModel(&m_dateEventsModel);

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

void DataManager::loadEntity(const QJsonObject &entityObj)
{
    Gaia::ModelData eventData;

    const auto packageId = checkAndValue(entityObj, Tags::id).toString();

    const auto agentObj = checkAndValue(entityObj, Tags::agent).toObject();
    const auto companyId = checkAndValue(agentObj, Tags::companyId).toString();
    const auto agentRole = DataGlobals::userTypeFromString(
                checkAndValue(agentObj, Tags::role).toString());

    const auto date = QDateTime::fromSecsSinceEpoch(
                checkAndValue(entityObj, Tags::timestamp).toVariant().value<qint64>());
    const auto action = DataGlobals::supplyChainActionFromString(
                checkAndValue(entityObj, Tags::action).toString());
    const auto properties = checkAndValue(entityObj, Tags::properties).toVariant().toHash();

    eventData.append(QVariantList({ packageId,
                                    QVariant::fromValue(agentRole),
                                    companyId,
                                    date,
                                    QVariant::fromValue(action),
                                    properties
                                  } ));

    m_eventModel.appendData(eventData);
}
