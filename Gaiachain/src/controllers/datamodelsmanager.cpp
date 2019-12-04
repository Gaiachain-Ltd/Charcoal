#include "datamodelsmanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDateTime>

namespace {
const QLatin1String EventsTableName = QLatin1String("Events");
const QLatin1String RelationsTableName = QLatin1String("Relations");
const QLatin1String UnusedIdsTableName = QLatin1String("UnusedIds");

const QLatin1String ProducersTableName = QLatin1String("Producers");
const QLatin1String BuyersTableName = QLatin1String("Buyers");
const QLatin1String TransportersTableName = QLatin1String("Transporters");
const QLatin1String DestinationsTableName = QLatin1String("Destinations");
}

DataModelsManager::DataModelsManager(QObject *parent)
    : AbstractManager(parent)
{
    connect(&m_latestRangePackagesTypeSearchEventsModel, &LatestRangeEventsProxyModel::fetchEvents,
            this, [this](int count, const QDateTime &from) { emit countEventsNeeded(count, from, m_searchEventsModel.keyword()); });
}

void DataModelsManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("producersModel"), &m_producersModel);
    engine.rootContext()->setContextProperty(QStringLiteral("buyersModel"), &m_buyersModel);
    engine.rootContext()->setContextProperty(QStringLiteral("transportersModel"), &m_transportersModel);
    engine.rootContext()->setContextProperty(QStringLiteral("destinationsModel"), &m_destinationsModel);

    engine.rootContext()->setContextProperty(QStringLiteral("eventsModel"), &m_eventsSourceModel);
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

void DataModelsManager::setupModels(QSqlDatabase db)
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

void DataModelsManager::updateCooperativeId(const QString &cooperativeId)
{
    m_cooperativeEventsModel.setCooperativeId(cooperativeId);
    m_cooperativeFilteringEventsModel.setCooperativeId(cooperativeId);
}

void DataModelsManager::addLocalAction(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                                       const Enums::UserType &userType, const QString &cooperativeId, const QVariantMap &properties)
{
    ProcessCounter p(this);

    m_eventsSourceModel.appendData({ Gaia::ModelEntry {
                                         packageId,
                                         QVariant::fromValue(action),
                                         timestamp,
                                         QVariant::fromValue(userType),
                                         cooperativeId,
                                         properties,
                                         0.0,    // location not handled yet
                                         0.0,    // location not handled yet
                                         true
                                     }, });
}

void DataModelsManager::updateLocalAction(const QString &packageId, const Enums::SupplyChainAction &action)
{
    ProcessCounter p(this);

    m_eventsSourceModel.updateData({ packageId, QVariant::fromValue(action) }, { { EventModel::Columns::IsLocal, false } });
}

void DataModelsManager::removeLocalAction(const QString &packageId, const Enums::SupplyChainAction &action)
{
    ProcessCounter p(this);

    m_eventsSourceModel.removeData({ packageId, QVariant::fromValue(action) });
}

void DataModelsManager::getPackageData(const QString &packageId) const
{
    ProcessCounter p(this);

    auto package = PackageData{};
    package.id = packageId;
    package.relatedPackages = m_relationsListModel.relatedPackages(packageId);
    m_packageDataModel.fillPackageData(package);

    emit packageData(package);
}

void DataModelsManager::processAdditionalData(const QMap<Enums::AdditionalDataType, Gaia::ModelData> &modelsData)
{
    ProcessCounter p(this);

    // producers
    if (modelsData.contains(Enums::AdditionalDataType::ProducersData)) {
        auto modelData = modelsData.value(Enums::AdditionalDataType::ProducersData);
        removeExistingProducers(modelData);
        m_producersModel.appendData(modelData);
    }

    // other
    static const auto AdditionalDataModels = std::array<std::tuple<Enums::AdditionalDataType, AbstractModel*, QLatin1String>, 3> {
        std::make_tuple(Enums::AdditionalDataType::BuyersData, &m_buyersModel, BuyersTableName),
        std::make_tuple(Enums::AdditionalDataType::TransportersData, &m_transportersModel, TransportersTableName),
        std::make_tuple(Enums::AdditionalDataType::DestinationsData, &m_destinationsModel, DestinationsTableName)
    };

    std::for_each(AdditionalDataModels.cbegin(), AdditionalDataModels.cend(),
                  [this, &modelsData](const auto &additionalDataModel) {
        auto [ type, model, tableName ] = additionalDataModel;
        Q_ASSERT(model);

        if (modelsData.contains(type)) {
            auto modelData = modelsData.value(type);
            this->removeExistingNameData(modelData, model, tableName);
            model->appendData(modelData);
        }
    });
}

void DataModelsManager::processEntitiesInfo(const Gaia::ModelData &eventsInfo)
{
    ProcessCounter p(this);

    // events
    auto missingEventsInfo = eventsInfo;
    auto missingEventsIds = QStringList{};
    removeExistingEvents(missingEventsInfo);
    std::transform(missingEventsInfo.constBegin(), missingEventsInfo.constEnd(), std::back_inserter(missingEventsIds),
                   [](const auto &entry) { return entry.isEmpty() ? QString{} : entry.first().toString(); });

    missingEventsIds.removeDuplicates();
    if (!missingEventsIds.isEmpty()) {
        emit eventsNeeded(missingEventsIds);
    }

    // relations
    auto missingRelationsInfo = eventsInfo;
    removeExistingRelations(missingRelationsInfo, false);
    auto missingRelationsIds = QStringList{};
    std::transform(missingRelationsInfo.constBegin(), missingRelationsInfo.constEnd(), std::back_inserter(missingRelationsIds),
                   [](const auto &entry) { return entry.isEmpty() ? QString{} : entry.first().toString(); });

    missingRelationsIds.removeDuplicates();
    if (!missingRelationsIds.isEmpty()) {
        emit relationsNeeded(missingRelationsIds);
    }
}

void DataModelsManager::processEntities(const Gaia::ModelData &modelData)
{
    ProcessCounter p(this);

    auto missingEvents = modelData;
    removeExistingEvents(missingEvents);
    m_eventsSourceModel.appendData(missingEvents);
}

void DataModelsManager::processRelations(const Gaia::ModelData &modelData)
{
    ProcessCounter p(this);

    auto missingRelations = modelData;
    removeExistingRelations(missingRelations, true);
    m_relationsSourceModel.appendData(missingRelations);
}

void DataModelsManager::processUnusedLotIds(const Gaia::ModelData &modelData)
{
    ProcessCounter p(this);

    auto missingUnusedLotIds = modelData;
    removeExistingUnusedLotIds(missingUnusedLotIds);
    m_unusedLotIdsModel.appendData(missingUnusedLotIds);
}

void DataModelsManager::getOfflineActions() const
{
    ProcessCounter p(this);

    emit offlineActions(m_localOnlyEventsModel.getData());
}

void DataModelsManager::removeExistingProducers(Gaia::ModelData &modelData)
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

void DataModelsManager::removeExistingNameData(Gaia::ModelData &modelData, AbstractModel *model, const QLatin1String &tableName)
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

void DataModelsManager::removeExistingEvents(Gaia::ModelData &modelData)
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

void DataModelsManager::removeExistingRelations(Gaia::ModelData &modelData, bool fullCheck)
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

void DataModelsManager::removeExistingUnusedLotIds(Gaia::ModelData &modelData)
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
