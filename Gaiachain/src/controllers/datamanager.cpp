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
    engine.rootContext()->setContextProperty(QStringLiteral("companyCalendarModel"), &m_companyCalendarModel);
    engine.rootContext()->setContextProperty(QStringLiteral("packagesCompanyCalendarModel"), &m_packagesCalendarModel);
    engine.rootContext()->setContextProperty(QStringLiteral("dateEventsModel"), &m_dateEventsModel);
    engine.rootContext()->setContextProperty(QStringLiteral("latestDateEventsModel"), &m_latestDateEventsModel);
    engine.rootContext()->setContextProperty(QStringLiteral("companyLatestDateEventsModel"), &m_companyLatestDateEventsModel);
    engine.rootContext()->setContextProperty(QStringLiteral("latestEventsModel"), &m_latestEventsModel);
    engine.rootContext()->setContextProperty(QStringLiteral("searchLatestEventsModel"), &m_searchLatestEventsModel);
    engine.rootContext()->setContextProperty(QStringLiteral("companySearchLatestEventsModel"), &m_companySearchLatestEventsModel);
    engine.rootContext()->setContextProperty(QStringLiteral("packagesTypeCompanySearchLatestEventsModel"), &m_packagesTypeCompanySearchLatestEventsModel);
}

void DataManager::updateCompanyId(const QString &companyId)
{
    m_companyCalendarModel.setCompanyId(companyId);
    m_companyLatestDateEventsModel.setCompanyId(companyId);
    m_companySearchLatestEventsModel.setCompanyId(companyId);
}

void DataManager::clearModels()
{
    m_relationsModel.clear();
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

void DataManager::onRelationsLoaded(const QJsonObject &relations)
{
    for (const auto &key : relations.keys()) {
        for (auto value : relations.value(key).toArray()) {
            m_relationsModel.insert(key, value.toString());
        }
    }
}

PackageData DataManager::getPackageData(const QString &packageId) const
{
    auto packageData = PackageData{};
    packageData.id = packageId;
    packageData.relatedPackages = m_relationsModel.values(packageId);
    m_packageDataModel.fillPackageData(packageData);

    return packageData;
}

void DataManager::setupModels()
{
    m_packageDataModel.setSourceModel(&m_eventModel);

    m_calendarModel.setSourceModel(&m_eventModel);
    m_companyCalendarModel.setSourceModel(&m_calendarModel);
    m_packagesCalendarModel.setSourceModel(&m_companyCalendarModel);

    m_dateEventsModel.setSourceModel(&m_eventModel);
    m_latestDateEventsModel.setSourceModel(&m_dateEventsModel);
    m_companyLatestDateEventsModel.setSourceModel(&m_latestDateEventsModel);

    m_latestEventsModel.setSourceModel(&m_eventModel);
    m_searchLatestEventsModel.setSourceModel(&m_latestEventsModel);
    m_companySearchLatestEventsModel.setSourceModel(&m_searchLatestEventsModel);
    m_packagesTypeCompanySearchLatestEventsModel.setSourceModel(&m_companySearchLatestEventsModel);
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
