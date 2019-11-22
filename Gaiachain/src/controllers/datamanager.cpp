#include "datamanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDateTime>
#include <QVariant>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

#include "../common/enums.h"
#include "../common/globals.h"
#include "../common/tags.h"
#include "../common/logs.h"
#include "../common/dataglobals.h"
#include "../helpers/utility.h"
#include "../helpers/requestshelper.h"

#include <QLoggingCategory>
Q_LOGGING_CATEGORY(dataManager, "data.manager")

namespace {
const QList<Enums::SupplyChainAction> OfflineActions = { Enums::SupplyChainAction::Harvest, Enums::SupplyChainAction::GrainProcessing };
}

DataManager::DataManager(QObject *parent)
    : AbstractManager(parent)
{
    connect(&m_latestRangePackagesTypeSearchEventsModel, &LatestRangeEventsProxyModel::fetchEvents,
            this, [this](int count, const QDateTime &from) { fetchCountEvents(count, from, m_searchEventsModel.keyword()); });
}

void DataManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("dataManager"), this);

    engine.rootContext()->setContextProperty(QStringLiteral("producersModel"), &m_producersModel);
    engine.rootContext()->setContextProperty(QStringLiteral("buyersModel"), &m_buyersModel);
    engine.rootContext()->setContextProperty(QStringLiteral("transportersModel"), &m_transportersModel);
    engine.rootContext()->setContextProperty(QStringLiteral("destinationsModel"), &m_destinationsModel);

    engine.rootContext()->setContextProperty(QStringLiteral("createdHarvestIdsModel"), &m_createdHarvestIdsModel);
    engine.rootContext()->setContextProperty(QStringLiteral("unusedLotIdsModel"), &m_unusedLotIdsModel);
    engine.rootContext()->setContextProperty(QStringLiteral("packageTypeCooperativeIdsModel"), &m_packageTypeCooperativeIdsModel);

    engine.rootContext()->setContextProperty(QStringLiteral("cooperativeFilteringEvents"), &m_cooperativeFilteringEventsModel);

    engine.rootContext()->setContextProperty(QStringLiteral("calendarModel"), &m_calendarModel);
    engine.rootContext()->setContextProperty(QStringLiteral("packagesCalendarModel"), &m_packagesCalendarModel);

    engine.rootContext()->setContextProperty(QStringLiteral("dateEventsModel"), &m_dateEventsModel);
    engine.rootContext()->setContextProperty(QStringLiteral("latestDateEventsModel"), &m_latestDateEventsModel);

    engine.rootContext()->setContextProperty(QStringLiteral("searchEventsModel"), &m_searchEventsModel);
    engine.rootContext()->setContextProperty(QStringLiteral("packagesTypeSearchEventsModel"), &m_packagesTypeSearchEventsModel);
    engine.rootContext()->setContextProperty(QStringLiteral("latestRangePackagesTypeSearchEventsModel"), &m_latestRangePackagesTypeSearchEventsModel);
}

void DataManager::updateUserData(const UserData &userData)
{
    m_userData = userData;
    updateCooperativeId();
}

void DataManager::onAdditionalDataLoaded(const QJsonObject &additionalData)
{
    Q_ASSERT(m_producersSourceModel && m_buyersSourceModel &&
             m_transportersSourceModel && m_destinationsSourceModel);

    auto modelData = Gaia::ModelData{};
    auto array = QJsonArray{};

    // producers
    array = additionalData.value(Tags::producers).toArray();
    std::transform(array.constBegin(), array.constEnd(), std::back_inserter(modelData),
                   std::bind(&DataManager::processProducer, this, std::placeholders::_1));
    removeExistingProducers(modelData);
    m_producersSourceModel->appendData(modelData);

    modelData.clear();

    // other
    static const auto AdditionalDataModels = QList<QPair<QLatin1String, AbstractModel*>> {
        { Tags::buyers, m_buyersSourceModel.data() },
        { Tags::transporters, m_transportersSourceModel.data() },
        { Tags::destinations, m_destinationsSourceModel.data() }
    };

    std::for_each(AdditionalDataModels.constBegin(), AdditionalDataModels.constEnd(),
                  [this, &additionalData, &array, &modelData](const auto &dataModel) {
        Q_ASSERT(dataModel.second);

        array = additionalData.value(dataModel.first).toArray();
        std::transform(array.constBegin(), array.constEnd(), std::back_inserter(modelData),
                       std::bind(&DataManager::processNameData, this, std::placeholders::_1));
        this->removeExistingNameData(modelData, dataModel.second);
        dataModel.second->appendData(modelData);

        modelData.clear();
    });
}

void DataManager::onEntitiesInfoLoaded(const QJsonArray &entitiesInfo)
{
    auto eventsInfo = Gaia::ModelData{};
    std::transform(entitiesInfo.constBegin(), entitiesInfo.constEnd(), std::back_inserter(eventsInfo),
                   std::bind(&DataManager::processEventInfo, this, std::placeholders::_1));

    fetchMissingEvents(eventsInfo);
    dataRequestProcessed();
}

void DataManager::onEntitiesLoaded(const QJsonArray &entities)
{
    Q_ASSERT(m_eventsSourceModel);

    auto modelData = Gaia::ModelData{};
    std::transform(entities.constBegin(), entities.constEnd(), std::back_inserter(modelData),
                   std::bind(&DataManager::processEvent, this, std::placeholders::_1));

    removeExistingEvents(modelData);
    m_eventsSourceModel->appendData(modelData);

    dataRequestProcessed();
}

void DataManager::onRelationsLoaded(const QJsonArray &relations)
{
    Q_ASSERT(m_relationsSourceModel);

    auto modelData = Gaia::ModelData{};
    auto modelDataArrays = QVector<Gaia::ModelData>{};
    std::transform(relations.constBegin(), relations.constEnd(), std::back_inserter(modelDataArrays),
                   std::bind(&DataManager::processRelations, this, std::placeholders::_1));
    std::for_each(modelDataArrays.begin(), modelDataArrays.end(),
                  [&modelData](const auto &item) { std::move(item.begin(), item.end(), std::back_inserter(modelData)); });

    removeExistingRelations(modelData, true);
    m_relationsSourceModel->appendData(modelData);

    dataRequestProcessed();
}

void DataManager::onCreatedHarvestIdsLoaded(const QJsonArray &idsArray)
{
    auto eventsInfo = Gaia::ModelData{};
    std::transform(idsArray.constBegin(), idsArray.constEnd(), std::back_inserter(eventsInfo),
                   std::bind(&DataManager::processCreatedHarvestId, this, std::placeholders::_1));

    fetchMissingEvents(eventsInfo);
}

void DataManager::onUnusedLotIdsLoaded(const QJsonArray &ids)
{
    Q_ASSERT(m_unusedIdsSourceModel);

    auto modelData = Gaia::ModelData{};
    std::transform(ids.constBegin(), ids.constEnd(), std::back_inserter(modelData),
                   std::bind(&DataManager::processUnusedLotId, this, std::placeholders::_1));

    removeExistingUnusedLotIds(modelData);
    m_unusedIdsSourceModel->appendData(modelData);
}

void DataManager::updateCooperativeId()
{
    m_cooperativeEventsModel.setCooperativeId(m_userData.cooperativeId);
    m_cooperativeFilteringEventsModel.setCooperativeId(m_userData.cooperativeId);
}

void DataManager::dataRequestSent()
{
    if (++m_dataRequestsCount == 1) {   // first action
        emit collectingDataChanged(true);
    }
}

void DataManager::dataRequestProcessed()
{
    Q_ASSERT(m_dataRequestsCount > 0);
    if (--m_dataRequestsCount == 0) {   // last action
        emit collectingDataChanged(false);
    }
}

void DataManager::fetchMissingEvents(const Gaia::ModelData &eventsInfo)
{
    auto missingEventsInfo = eventsInfo;
    auto missingEventsIds = QStringList{};
    removeExistingEvents(missingEventsInfo);
    std::transform(missingEventsInfo.constBegin(), missingEventsInfo.constEnd(), std::back_inserter(missingEventsIds),
                   [](const auto &entry) { return entry.isEmpty() ? QString{} : entry.first().toString(); });
    missingEventsIds.removeDuplicates();

    auto missingRelationsInfo = eventsInfo;
    removeExistingRelations(missingRelationsInfo, false);
    auto missingRelationsIds = QStringList{};
    std::transform(missingRelationsInfo.constBegin(), missingRelationsInfo.constEnd(), std::back_inserter(missingRelationsIds),
                   [](const auto &entry) { return entry.isEmpty() ? QString{} : entry.first().toString(); });

    if (!missingEventsIds.isEmpty()) {
        dataRequestSent();
        emit eventsNeeded(missingEventsIds);
    }
    if (!missingRelationsIds.isEmpty()) {
        dataRequestSent();
        emit relationsNeeded(missingRelationsIds);
    }
}

PackageData DataManager::getPackageData(const QString &packageId) const
{
    auto packageData = PackageData{};
    packageData.id = packageId;
    packageData.relatedPackages = m_relationsListModel.relatedPackages(packageId);
    m_packageDataModel.fillPackageData(packageData);

    return packageData;
}

bool DataManager::collectingData() const
{
    return (m_dataRequestsCount > 0);
}

void DataManager::addAction(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties, const QByteArray &codeData)
{
    handleActionAdd(packageId, action, timestamp, properties);
    emit addActionRequest(packageId, action, timestamp, properties, codeData);
}

void DataManager::addAction(const QByteArray &codeData, const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties)
{
    emit addActionRequest(codeData, action, timestamp, properties);
}

void DataManager::addAction(const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties, const QByteArray &codeData)
{
    if (action == Enums::SupplyChainAction::Harvest) {
        auto id = generateHarvestId(timestamp.date(), properties.value(PackageData::ParcelCode).toString());
        addAction(id, action, timestamp, properties, codeData);
    } else {
        emit addActionRequest(action, timestamp, properties, codeData);
    }
}

void DataManager::fetchEventData(const QString &packageId, const Enums::PackageType &type)
{
    auto eventsInfo = Gaia::ModelData{};

    const auto packageActions = DataGlobals::packageActions(type);
    std::transform(packageActions.constBegin(), packageActions.constEnd(), std::back_inserter(eventsInfo),
                   [&packageId](const auto &action) { return Gaia::ModelEntry{ packageId, QVariant::fromValue(action) }; });

    fetchMissingEvents(eventsInfo);
}

void DataManager::fetchRangeEvents(const QDateTime &from, const QDateTime &to, const QString &keyword)
{
    dataRequestSent();
    emit eventsInfoNeeded(from, to, keyword);
}

void DataManager::fetchCountEvents(int count, const QDateTime &from, const QString &keyword)
{
    dataRequestSent();
    emit eventsInfoNeeded(count, from, keyword);
}

void DataManager::onActionAdded(const QString &packageId, const Enums::SupplyChainAction &action)
{
    Q_ASSERT(m_eventsSourceModel);

    m_eventsSourceModel->updateLocal(packageId, action, false);
}

void DataManager::onDataRequestError()
{
    dataRequestProcessed();
}

void DataManager::setupModels(QSqlDatabase db)
{
    m_existsQueryModel.reset(new ExistsQueryModel(db));

    m_producersSourceModel.reset(new ProducerModel(db));
    m_buyersSourceModel.reset(new NameModel(QLatin1String("Buyers"), db));
    m_transportersSourceModel.reset(new NameModel(QLatin1String("Transporters"), db));
    m_destinationsSourceModel.reset(new NameModel(QLatin1String("Destinations"), db));

    m_producersModel.setSourceModel(m_producersSourceModel.data());
    m_buyersModel.setSourceModel(m_buyersSourceModel.data());
    m_transportersModel.setSourceModel(m_transportersSourceModel.data());
    m_destinationsModel.setSourceModel(m_destinationsSourceModel.data());

    // -------------------------------------------------------------

    m_eventsSourceModel.reset(new EventModel(db));
    m_relationsSourceModel.reset(new RelationModel(db));
    m_unusedIdsSourceModel.reset(new UnusedIdsModel(db));

    m_createdHarvestIdsModel.setSourceModel(&m_cooperativeEventsModel);
    m_unusedLotIdsModel.setSourceModel(m_unusedIdsSourceModel.data());

    m_cooperativeEventsModel.setSourceModel(m_eventsSourceModel.data());
    m_packageTypeCooperativeIdsModel.setSourceModel(&m_cooperativeEventsModel);

    m_cooperativeFilteringEventsModel.setSourceModel(m_eventsSourceModel.data());
    m_calendarModel.setSourceModel(&m_cooperativeFilteringEventsModel);
    m_packagesCalendarModel.setSourceModel(&m_calendarModel);

    m_dateEventsModel.setSourceModel(&m_calendarModel);
    m_latestDateEventsModel.setSourceModel(&m_dateEventsModel);

    m_searchEventsModel.setSourceModel(&m_cooperativeFilteringEventsModel);
    m_packagesTypeSearchEventsModel.setSourceModel(&m_searchEventsModel);
    m_latestRangePackagesTypeSearchEventsModel.setSourceModel(&m_packagesTypeSearchEventsModel);

    m_packageDataModel.setSourceModel(m_eventsSourceModel.data());
    m_relationsListModel.setSourceModel(m_relationsSourceModel.data());
}

QJsonValue DataManager::checkAndValue(const QJsonObject &object, const QLatin1String tag)
{
    if (!object.contains(tag)) {
        qCWarning(dataManager) << "Tag" << tag << "is missing in an object data!";
        return {};
    }
    return object.value(tag);
}

void DataManager::handleActionAdd(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties)
{
    Q_ASSERT(m_eventsSourceModel);

    if (OfflineActions.contains(action) &&
            !m_userData.isAnonymous()) {
        m_eventsSourceModel->appendData({ Gaia::ModelEntry {
                                              packageId,
                                              QVariant::fromValue(action),
                                              timestamp,
                                              QVariant::fromValue(m_userData.type),
                                              m_userData.cooperativeId,
                                              properties,
                                              0.0,    // location not handled yet
                                              0.0,    // location not handled yet
                                              true
                                          }, });
    }
}

QString DataManager::generateHarvestId(const QDate &date, const QString &parcelCode)
{
    return date.toString(QStringLiteral("%1/d-M-yyyy")).arg(parcelCode);
}

Gaia::ModelEntry DataManager::processProducer(const QJsonValue &value)
{
    auto object = value.toObject();

    const auto producerId = checkAndValue(object, Tags::id).toString();
    const auto name = checkAndValue(object, Tags::name).toString();
    const auto village = checkAndValue(object, Tags::village).toString();
    const auto parcels = checkAndValue(object, Tags::parcels).toVariant().toStringList();

    return Gaia::ModelEntry {
                producerId,
                name,
                village,
                parcels
    };
}

Gaia::ModelEntry DataManager::processNameData(const QJsonValue &value)
{
    return Gaia::ModelEntry { value.toString() };
}

Gaia::ModelEntry DataManager::processEventInfo(const QJsonValue &value)
{
    auto object = value.toObject();

    const auto packageId = checkAndValue(object, Tags::id).toString();
    const auto action = RequestsHelper::supplyChainActionFromString(
                checkAndValue(object, Tags::action).toString());

    return Gaia::ModelEntry {
                packageId,
                QVariant::fromValue(action),
    };
}

Gaia::ModelEntry DataManager::processEvent(const QJsonValue &value)
{
    auto object = value.toObject();

    const auto packageId = checkAndValue(object, Tags::id).toString();
    const auto action = RequestsHelper::supplyChainActionFromString(
                checkAndValue(object, Tags::action).toString());

    const auto date = QDateTime::fromSecsSinceEpoch(
                checkAndValue(object, Tags::timestamp).toVariant().value<qint64>());
    const auto properties = checkAndValue(object, Tags::properties).toObject().toVariantMap();

    const auto agentObj = checkAndValue(object, Tags::agent).toObject();
    const auto cooperativeId = checkAndValue(agentObj, Tags::cooperativeId).toString();
    const auto agentRole = RequestsHelper::userTypeFromString(
                checkAndValue(agentObj, Tags::role).toString());

    return Gaia::ModelEntry {
                packageId,
                QVariant::fromValue(action),
                date,
                QVariant::fromValue(agentRole),
                cooperativeId,
                properties,
                0.0,    // location not handled yet
                0.0     // location not handled yet
    };
}

Gaia::ModelData DataManager::processRelations(const QJsonValue &value)
{
    auto object = value.toObject();

    const auto id = checkAndValue(object, Tags::id).toString();
    const auto relatedIds = checkAndValue(object, Tags::ids).toVariant().toStringList();

    auto modelData = Gaia::ModelData{};
    std::transform(relatedIds.begin(), relatedIds.end(), std::back_inserter(modelData),
                   [&id](const auto &relatedId) { return Gaia::ModelEntry{{ id, relatedId }, }; });
    return modelData;
}

Gaia::ModelEntry DataManager::processCreatedHarvestId(const QJsonValue &value)
{
    return { value.toString(), QVariant::fromValue(Enums::SupplyChainAction::Harvest) };
}

Gaia::ModelEntry DataManager::processUnusedLotId(const QJsonValue &value)
{
    return { value.toString(), QVariant::fromValue(Enums::PackageType::Lot) };
}

void DataManager::removeExistingProducers(Gaia::ModelData &modelData)
{
    Q_ASSERT(m_existsQueryModel && m_producersSourceModel);

    static const auto ConditionsFields = QStringList{
            m_producersSourceModel->roleNames().value(ProducerModel::Columns::ProducerId) };

    m_existsQueryModel->prepareQuery(m_producersSourceModel->tableName(), ConditionsFields);

    modelData.erase(std::remove_if(modelData.begin(), modelData.end(),
                                   [this](const auto &entry) {
        return entry.isEmpty() ? false
                               : m_existsQueryModel->exists({ entry.first() });
    }), modelData.end());
}

void DataManager::removeExistingNameData(Gaia::ModelData &modelData, AbstractModel *model)
{
    Q_ASSERT(m_existsQueryModel && model);

    static const auto ConditionsFields = QStringList{ NameModel::columnName() };

    m_existsQueryModel->prepareQuery(model->tableName(), ConditionsFields);

    modelData.erase(std::remove_if(modelData.begin(), modelData.end(),
                                   [this](const auto &entry) {
        return entry.isEmpty() ? false
                               : m_existsQueryModel->exists({ entry.first() });
    }), modelData.end());
}

void DataManager::removeExistingEvents(Gaia::ModelData &modelData)
{
    Q_ASSERT(m_existsQueryModel && m_eventsSourceModel);

    static const auto ConditionsFields = QStringList{
            m_eventsSourceModel->roleNames().value(EventModel::Columns::PackageId),
            m_eventsSourceModel->roleNames().value(EventModel::Columns::Action) };

    m_existsQueryModel->prepareQuery(m_eventsSourceModel->tableName(), ConditionsFields);

    modelData.erase(std::remove_if(modelData.begin(), modelData.end(),
                                   [this](const auto &entry) {
        return entry.size() < 2 ? false
                                : m_existsQueryModel->exists({ entry.at(0), entry.at(1).toInt() });
    }), modelData.end());
}

void DataManager::removeExistingRelations(Gaia::ModelData &modelData, bool fullCheck)
{
    Q_ASSERT(m_existsQueryModel && m_relationsSourceModel);

    static const auto ConditionsFields = QStringList{
            m_relationsSourceModel->roleNames().value(RelationModel::Columns::PackageId) };
    static const auto FullConditionsFields = QStringList{
            m_relationsSourceModel->roleNames().value(RelationModel::Columns::PackageId),
            m_relationsSourceModel->roleNames().value(RelationModel::Columns::RelatedId) };

    m_existsQueryModel->prepareQuery(m_relationsSourceModel->tableName(),
                                     fullCheck ? FullConditionsFields : ConditionsFields);

    modelData.erase(std::remove_if(modelData.begin(), modelData.end(),
                                   [this, fullCheck](const auto &entry) {
        if (fullCheck) {
            return entry.size() < 2 ? false
                                    : m_existsQueryModel->exists({ entry.at(0), entry.at(1) });
        } else {
            return entry.isEmpty() ? false
                                   : m_existsQueryModel->exists({ entry.first() });
        }
    }), modelData.end());
}

void DataManager::removeExistingUnusedLotIds(Gaia::ModelData &modelData)
{
    Q_ASSERT(m_existsQueryModel && m_unusedIdsSourceModel);

    static const auto ConditionsFields = QStringList{
            m_unusedIdsSourceModel->roleNames().value(UnusedIdsModel::Columns::PackageId) };

    m_existsQueryModel->prepareQuery(m_unusedIdsSourceModel->tableName(), ConditionsFields);

    modelData.erase(std::remove_if(modelData.begin(), modelData.end(),
                                   [this](const auto &entry) {
        return entry.isEmpty() ? false
                               : m_existsQueryModel->exists({ entry.first() });
    }), modelData.end());
}
