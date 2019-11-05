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
#include "../common/tags.h"
#include "../common/logs.h"
#include "../helpers/utility.h"
#include "../helpers/requestshelper.h"

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

    engine.rootContext()->setContextProperty(QStringLiteral("packageTypeCooperativeIdsModel"), &m_packageTypeCooperativeIdsModel);
    engine.rootContext()->setContextProperty(QStringLiteral("unusedLotIdsModel"), &m_unusedLotIdsModel);

    engine.rootContext()->setContextProperty(QStringLiteral("cooperativeFilteringEvents"), &m_cooperativeFilteringEventsModel);

    engine.rootContext()->setContextProperty(QStringLiteral("calendarModel"), &m_calendarModel);
    engine.rootContext()->setContextProperty(QStringLiteral("packagesCalendarModel"), &m_packagesCalendarModel);

    engine.rootContext()->setContextProperty(QStringLiteral("dateEventsModel"), &m_dateEventsModel);
    engine.rootContext()->setContextProperty(QStringLiteral("latestDateEventsModel"), &m_latestDateEventsModel);

    engine.rootContext()->setContextProperty(QStringLiteral("latestEventsModel"), &m_latestEventsModel);
    engine.rootContext()->setContextProperty(QStringLiteral("searchLatestEventsModel"), &m_searchLatestEventsModel);
    engine.rootContext()->setContextProperty(QStringLiteral("packagesTypeSearchLatestEventsModel"), &m_packagesTypeSearchLatestEventsModel);

    engine.rootContext()->setContextProperty(QStringLiteral("producersModel"), &m_producersModel);
    engine.rootContext()->setContextProperty(QStringLiteral("buyersModel"), &m_buyersModel);
    engine.rootContext()->setContextProperty(QStringLiteral("transportersModel"), &m_transportersModel);
    engine.rootContext()->setContextProperty(QStringLiteral("destinationsModel"), &m_destinationsModel);
}

void DataManager::updateCooperativeId(const QString &cooperativeId)
{
    m_cooperativeEventsModel.setCooperativeId(cooperativeId);
    m_cooperativeFilteringEventsModel.setCooperativeId(cooperativeId);
}

void DataManager::clearModels()
{
    m_relationsModel.clear();
    m_eventModel.clearModel();
}

void DataManager::onAdditionalDataLoaded(const QJsonObject &additionalData)
{
    auto producersModelData = Gaia::ModelData{};
    for (auto producerValue : additionalData.value(Tags::producers).toArray()) {
        auto producerObj = producerValue.toObject();
        producersModelData.append({ producerObj.value(Tags::id).toString(),
                                    producerObj.value(Tags::name).toString(),
                                    producerObj.value(Tags::village).toString(),
                                    producerObj.value(Tags::parcels).toVariant().toStringList() });
    }
    m_producersModel.appendData(producersModelData);

    m_buyersModel.setStringList(additionalData.value(Tags::buyers).toVariant().toStringList());
    m_transportersModel.setStringList(additionalData.value(Tags::transporters).toVariant().toStringList());
    m_destinationsModel.setStringList(additionalData.value(Tags::destinations).toVariant().toStringList());
}

void DataManager::onRelationsLoaded(const QJsonObject &relations)
{
    for (const auto &key : relations.keys()) {
        for (auto value : relations.value(key).toArray()) {
            m_relationsModel.insert(key, value.toString());
        }
    }
}

void DataManager::onEntitiesLoaded(const QJsonArray &entities)
{
    std::for_each(entities.constBegin(), entities.constEnd(),
                  [this](const QJsonValue &value) {
        loadEntity(value.toObject());
    });
}

void DataManager::onUnusedLotIdsLoaded(const QJsonArray &ids)
{
    m_unusedLotIdsModel.setStringList(QVariant::fromValue(ids.toVariantList()).toStringList());
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

    m_cooperativeEventsModel.setSourceModel(&m_eventModel);
    m_packageTypeCooperativeIdsModel.setSourceModel(&m_cooperativeEventsModel);

    m_cooperativeFilteringEventsModel.setSourceModel(&m_eventModel);
    m_calendarModel.setSourceModel(&m_cooperativeFilteringEventsModel);
    m_packagesCalendarModel.setSourceModel(&m_calendarModel);

    m_dateEventsModel.setSourceModel(&m_calendarModel);
    m_latestDateEventsModel.setSourceModel(&m_dateEventsModel);

    m_latestEventsModel.setSourceModel(&m_cooperativeFilteringEventsModel);
    m_searchLatestEventsModel.setSourceModel(&m_latestEventsModel);
    m_packagesTypeSearchLatestEventsModel.setSourceModel(&m_searchLatestEventsModel);
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
    const auto cooperativeId = checkAndValue(agentObj, Tags::cooperativeId).toString();
    const auto agentRole = RequestsHelper::userTypeFromString(
                checkAndValue(agentObj, Tags::role).toString());

    const auto date = QDateTime::fromSecsSinceEpoch(
                checkAndValue(entityObj, Tags::timestamp).toVariant().value<qint64>());
    const auto action = RequestsHelper::supplyChainActionFromString(
                checkAndValue(entityObj, Tags::action).toString());
    const auto properties = checkAndValue(entityObj, Tags::properties).toVariant().toHash();

    eventData.append(QVariantList({ packageId,
                                    QVariant::fromValue(agentRole),
                                    cooperativeId,
                                    date,
                                    QVariant::fromValue(action),
                                    properties
                                  } ));

    m_eventModel.appendData(eventData);
}
