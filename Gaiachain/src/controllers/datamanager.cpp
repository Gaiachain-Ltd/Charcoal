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
#include "../helpers/packagedataproperties.h"

#include <QLoggingCategory>
Q_LOGGING_CATEGORY(dataManager, "data.manager")

namespace {
const QLatin1String EventsTableName = QLatin1String("Events");
const QLatin1String RelationsTableName = QLatin1String("Relations");
const QLatin1String UnusedIdsTableName = QLatin1String("UnusedIds");

const QLatin1String ProducersTableName = QLatin1String("Producers");
const QLatin1String BuyersTableName = QLatin1String("Buyers");
const QLatin1String TransportersTableName = QLatin1String("Transporters");
const QLatin1String DestinationsTableName = QLatin1String("Destinations");
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

    engine.rootContext()->setContextProperty(QStringLiteral("localOnlyEventsModel"), &m_localOnlyEventsModel);

    engine.rootContext()->setContextProperty(QStringLiteral("lastActionHarvestModel"), &m_lastActionHarvestModel);
    engine.rootContext()->setContextProperty(QStringLiteral("lastActionGrainProcessingModel"), &m_lastActionGrainProcessingModel);
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
    auto modelData = Gaia::ModelData{};
    auto array = QJsonArray{};

    // producers
    array = additionalData.value(Tags::producers).toArray();
    std::transform(array.constBegin(), array.constEnd(), std::back_inserter(modelData),
                   std::bind(&DataManager::processProducer, this, std::placeholders::_1));
    removeExistingProducers(modelData);
    m_producersModel.appendData(modelData);

    modelData.clear();

    // other
    static const auto AdditionalDataModels = std::array<std::tuple<QLatin1String, AbstractModel*, QLatin1String>, 3> {
        std::make_tuple(Tags::buyers, &m_buyersModel, BuyersTableName),
        std::make_tuple(Tags::transporters, &m_transportersModel, TransportersTableName),
        std::make_tuple(Tags::destinations, &m_destinationsModel, DestinationsTableName)
    };

    std::for_each(AdditionalDataModels.cbegin(), AdditionalDataModels.cend(),
                  [this, &additionalData, &array, &modelData](const auto &dataModel) {
        auto [ tag, model, tableName ] = dataModel;
        Q_ASSERT(model);

        array = additionalData.value(tag).toArray();
        std::transform(array.constBegin(), array.constEnd(), std::back_inserter(modelData),
                       std::bind(&DataManager::processNameData, this, std::placeholders::_1));
        this->removeExistingNameData(modelData, model, tableName);
        model->appendData(modelData);

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
    auto modelData = Gaia::ModelData{};
    std::transform(entities.constBegin(), entities.constEnd(), std::back_inserter(modelData),
                   std::bind(&DataManager::processEvent, this, std::placeholders::_1));

    removeExistingEvents(modelData);
    m_eventsSourceModel.appendData(modelData);

    dataRequestProcessed();
}

void DataManager::onRelationsLoaded(const QJsonArray &relations)
{
    auto modelData = Gaia::ModelData{};
    auto modelDataArrays = QVector<Gaia::ModelData>{};
    std::transform(relations.constBegin(), relations.constEnd(), std::back_inserter(modelDataArrays),
                   std::bind(&DataManager::processRelations, this, std::placeholders::_1));
    std::for_each(modelDataArrays.begin(), modelDataArrays.end(),
                  [&modelData](const auto &item) { std::move(item.begin(), item.end(), std::back_inserter(modelData)); });

    removeExistingRelations(modelData, true);
    m_relationsSourceModel.appendData(modelData);

    dataRequestProcessed();
}

void DataManager::onUnusedLotIdsLoaded(const QJsonArray &ids)
{
    auto modelData = Gaia::ModelData{};
    std::transform(ids.constBegin(), ids.constEnd(), std::back_inserter(modelData),
                   std::bind(&DataManager::processUnusedLotId, this, std::placeholders::_1));

    removeExistingUnusedLotIds(modelData);
    m_unusedLotIdsModel.appendData(modelData);
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
    missingRelationsIds.removeDuplicates();

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

QString DataManager::generateHarvestId(const QDate &date, const QString &parcelCode) {
    Q_ASSERT(!parcelCode.isEmpty());
    return date.toString(QStringLiteral("%1/d-M-yyyy")).arg(parcelCode);
}

bool DataManager::collectingData() const
{
    return (m_dataRequestsCount > 0);
}

void DataManager::addAction(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties, const QByteArray &codeData)
{
    handleActionAdd(packageId, action, timestamp, properties);

    if (!m_addActionRequestSent.contains(packageId, action)) {
        m_addActionRequestSent.insert(packageId, action);

        dataRequestSent();
        emit addActionRequest(packageId, action, timestamp, properties, codeData);
    }
}

void DataManager::addAction(const QByteArray &codeData, const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties)
{
    dataRequestSent();
    emit addActionRequest(codeData, action, timestamp, properties);
}

void DataManager::addAction(const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties, const QByteArray &codeData)
{
    if (action == Enums::SupplyChainAction::Harvest) {
        auto id = generateHarvestId(timestamp.date(), properties.value(PackageDataProperties::ParcelCode).toString());
        addAction(id, action, timestamp, properties, codeData);
    } else {

        dataRequestSent();
        emit addActionRequest(action, timestamp, properties, codeData);
    }
}

void DataManager::sendOfflineActions()
{
    for (const auto &modelEntry : m_localOnlyEventsModel.getData()) {
        Q_ASSERT(modelEntry.size() >= 4);

        const auto packageId = modelEntry.at(0).toString();
        const auto action = modelEntry.at(1).value<Enums::SupplyChainAction>();

        if (!m_addActionRequestSent.contains(packageId, action)) {
            m_addActionRequestSent.insert(packageId, action);

            dataRequestSent();
            emit addActionRequest(packageId,
                                  action,
                                  modelEntry.at(2).toDateTime(),
                                  modelEntry.at(3).toMap());
        }
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

void DataManager::fetchLastActionPackageEvents(const Enums::SupplyChainAction &lastAction)
{
    dataRequestSent();
    emit lastActionEventsInfoNeeded(lastAction);
}

void DataManager::onActionAdded(const QString &packageId, const Enums::SupplyChainAction &action)
{
    m_addActionRequestSent.remove(packageId, action);
    m_eventsSourceModel.updateData({ packageId, QVariant::fromValue(action) }, { { EventModel::Columns::IsLocal, false } });

    dataRequestProcessed();
}

void DataManager::onActionAddError(const QString &packageId, const Enums::SupplyChainAction &action, const QNetworkReply::NetworkError &error)
{
    if (error == QNetworkReply::NetworkError::ContentConflictError) {
        m_eventsSourceModel.removeData({ packageId, QVariant::fromValue(action) });
    }
    m_addActionRequestSent.remove(packageId, action);

    dataRequestProcessed();
}

void DataManager::onDataRequestError()
{
    dataRequestProcessed();
}

void DataManager::setupModels(QSqlDatabase db)
{
    m_existsQueryModel.setDatabase(db);

    // -------------------------------------------------------------

    m_eventsDatabaseModel.reset(new SqlTableModel(EventsTableName, db));
    m_relationsDatabaseModel.reset(new SqlTableModel(RelationsTableName, db));
    m_unusedIdsDatabaseModel.reset(new SqlTableModel(UnusedIdsTableName, db));
    m_producersDatabaseModel.reset(new SqlTableModel(ProducersTableName, db));
    m_buyersDatabaseModel.reset(new SqlTableModel(BuyersTableName, db));
    m_transportersDatabaseModel.reset(new SqlTableModel(TransportersTableName, db));
    m_destinationsDatabaseModel.reset(new SqlTableModel(DestinationsTableName, db));

    // -------------------------------------------------------------

    m_eventsSourceModel.setSourceModel(m_eventsDatabaseModel.data());
    m_relationsSourceModel.setSourceModel(m_relationsDatabaseModel.data());
    m_unusedLotIdsModel.setSourceModel(m_unusedIdsDatabaseModel.data());
    m_producersModel.setSourceModel(m_producersDatabaseModel.data());
    m_buyersModel.setSourceModel(m_buyersDatabaseModel.data());
    m_transportersModel.setSourceModel(m_transportersDatabaseModel.data());
    m_destinationsModel.setSourceModel(m_destinationsDatabaseModel.data());

    // -------------------------------------------------------------

    m_cooperativeEventsModel.setSourceModel(&m_eventsSourceModel);
    m_lastActionHarvestModel.setSourceModel(&m_cooperativeEventsModel);
    m_lastActionGrainProcessingModel.setSourceModel(&m_cooperativeEventsModel);
    m_packageTypeCooperativeIdsModel.setSourceModel(&m_cooperativeEventsModel);

    m_localOnlyEventsModel.setSourceModel(&m_cooperativeEventsModel);

    m_cooperativeFilteringEventsModel.setSourceModel(&m_eventsSourceModel);
    m_calendarModel.setSourceModel(&m_cooperativeFilteringEventsModel);
    m_packagesCalendarModel.setSourceModel(&m_calendarModel);

    m_dateEventsModel.setSourceModel(&m_calendarModel);
    m_latestDateEventsModel.setSourceModel(&m_dateEventsModel);

    m_searchEventsModel.setSourceModel(&m_cooperativeFilteringEventsModel);
    m_packagesTypeSearchEventsModel.setSourceModel(&m_searchEventsModel);
    m_latestRangePackagesTypeSearchEventsModel.setSourceModel(&m_packagesTypeSearchEventsModel);

    m_packageDataModel.setSourceModel(&m_eventsSourceModel);
    m_relationsListModel.setSourceModel(&m_relationsSourceModel);
}

void DataManager::getInitialData()
{
    fetchLastActionPackageEvents(Enums::SupplyChainAction::Harvest);
    fetchLastActionPackageEvents(Enums::SupplyChainAction::GrainProcessing);
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
    if (DataGlobals::availableOfflineActions().contains(action) &&
            !m_userData.isAnonymous()) {
        m_eventsSourceModel.appendData({ Gaia::ModelEntry {
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

    const auto packageId = checkAndValue(object, Tags::packageId).toString();
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

    const auto packageId = checkAndValue(object, Tags::packageId).toString();
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

    const auto id = checkAndValue(object, Tags::packageId).toString();
    const auto relatedIds = checkAndValue(object, Tags::packageIds).toVariant().toStringList();

    auto modelData = Gaia::ModelData{};
    std::transform(relatedIds.begin(), relatedIds.end(), std::back_inserter(modelData),
                   [&id](const auto &relatedId) { return Gaia::ModelEntry{{ id, relatedId }, }; });
    return modelData;
}

Gaia::ModelEntry DataManager::processUnusedLotId(const QJsonValue &value)
{
    return { value.toString(), QVariant::fromValue(Enums::PackageType::Lot) };
}

void DataManager::removeExistingProducers(Gaia::ModelData &modelData)
{
    static const auto ConditionsFields = QStringList{
            m_producersModel.roleNames().value(ProducerModel::Columns::ProducerId) };

    m_existsQueryModel.prepareQuery(ProducersTableName, ConditionsFields);

    modelData.erase(std::remove_if(modelData.begin(), modelData.end(),
                                   [this](const auto &entry) {
        return entry.isEmpty() ? false
                               : m_existsQueryModel.exists({ entry.first() });
    }), modelData.end());
}

void DataManager::removeExistingNameData(Gaia::ModelData &modelData, AbstractModel *model, const QLatin1String &tableName)
{
    Q_ASSERT(model);
    if (model) {
        static const auto ConditionsFields = QStringList{ NameModel::columnName() };

        m_existsQueryModel.prepareQuery(tableName, ConditionsFields);

        modelData.erase(std::remove_if(modelData.begin(), modelData.end(),
                                       [this](const auto &entry) {
            return entry.isEmpty() ? false
                                   : m_existsQueryModel.exists({ entry.first() });
        }), modelData.end());
    }
}

void DataManager::removeExistingEvents(Gaia::ModelData &modelData)
{
    static const auto ConditionsFields = QStringList{
            m_eventsSourceModel.roleNames().value(EventModel::Columns::PackageId),
            m_eventsSourceModel.roleNames().value(EventModel::Columns::Action) };

    m_existsQueryModel.prepareQuery(EventsTableName, ConditionsFields);

    modelData.erase(std::remove_if(modelData.begin(), modelData.end(),
                                   [this](const auto &entry) {
        return entry.size() < 2 ? false
                                : m_existsQueryModel.exists({ entry.at(0), entry.at(1).toInt() });
    }), modelData.end());
}

void DataManager::removeExistingRelations(Gaia::ModelData &modelData, bool fullCheck)
{
    static const auto ConditionsFields = QStringList{
            m_relationsSourceModel.roleNames().value(RelationModel::Columns::PackageId) };
    static const auto FullConditionsFields = QStringList{
            m_relationsSourceModel.roleNames().value(RelationModel::Columns::PackageId),
            m_relationsSourceModel.roleNames().value(RelationModel::Columns::RelatedId) };

    m_existsQueryModel.prepareQuery(RelationsTableName,
                                    fullCheck ? FullConditionsFields : ConditionsFields);

    modelData.erase(std::remove_if(modelData.begin(), modelData.end(),
                                   [this, fullCheck](const auto &entry) {
        if (fullCheck) {
            return entry.size() < 2 ? false
                                    : m_existsQueryModel.exists({ entry.at(0), entry.at(1) });
        } else {
            return entry.isEmpty() ? false
                                   : m_existsQueryModel.exists({ entry.first() });
        }
    }), modelData.end());
}

void DataManager::removeExistingUnusedLotIds(Gaia::ModelData &modelData)
{
    static const auto ConditionsFields = QStringList{
            m_unusedLotIdsModel.roleNames().value(UnusedIdsModel::Columns::PackageId) };

    m_existsQueryModel.prepareQuery(UnusedIdsTableName, ConditionsFields);

    modelData.erase(std::remove_if(modelData.begin(), modelData.end(),
                                   [this](const auto &entry) {
        return entry.isEmpty() ? false
                               : m_existsQueryModel.exists({ entry.first() });
    }), modelData.end());
}
