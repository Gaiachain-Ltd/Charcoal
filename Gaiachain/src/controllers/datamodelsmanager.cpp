#include "datamodelsmanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDateTime>

#include "../models/proxy/sqlquerymodel.h"
#include "../models/proxy/companytypequery.h"
#include "../models/proxy/sortnamequery.h"

namespace {
const QLatin1String EventsTableName = QLatin1String("Events");
const QLatin1String RelationsTableName = QLatin1String("Relations");
const QLatin1String UnusedIdsTableName = QLatin1String("UnusedIds");

const QLatin1String ProducersTableName = QLatin1String("Producers");
const QLatin1String ParcelsTableName = QLatin1String("Parcels");
const QLatin1String CompaniesTableName = QLatin1String("Companies");
const QLatin1String DestinationsTableName = QLatin1String("Destinations");
}

DataModelsManager::DataModelsManager(QObject *parent)
    : AbstractManager(parent)
{
    connect(&m_latestRangePackagesTypeSearchEventsModel, &LatestRangeEventsProxyModel::fetchEvents,
            this, [this](int number, int offset) { emit limitEventsNeeded(number, offset, m_searchEventsModel.keyword()); });
}

void DataModelsManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("producersModel"), &m_producersViewModel);
    engine.rootContext()->setContextProperty(QStringLiteral("allParcelsModel"), &m_parcelsSourceModel);
    engine.rootContext()->setContextProperty(QStringLiteral("parcelsModel"), &m_parcelsViewModel);
    engine.rootContext()->setContextProperty(QStringLiteral("cooperativesModel"), &m_cooperativesViewModel);
    engine.rootContext()->setContextProperty(QStringLiteral("buyersModel"), &m_buyersViewModel);
    engine.rootContext()->setContextProperty(QStringLiteral("transportersModel"), &m_transportersViewModel);
    engine.rootContext()->setContextProperty(QStringLiteral("destinationsModel"), &m_destinationsViewModel);

    engine.rootContext()->setContextProperty(QStringLiteral("eventsModel"), &m_eventsSourceModel);
    engine.rootContext()->setContextProperty(QStringLiteral("localOnlyEventsModel"), &m_localOnlyEventsModel);

    engine.rootContext()->setContextProperty(QStringLiteral("lastActionHarvestModel"), &m_lastActionHarvestModel);
    engine.rootContext()->setContextProperty(QStringLiteral("lastActionGrainProcessingModel"), &m_lastActionGrainProcessingModel);
    engine.rootContext()->setContextProperty(QStringLiteral("lastActionSectionReceptionModel"), &m_lastActionSectionReceptionModel);
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
    m_parcelsDatabaseModel.reset(new SqlTableModel(ParcelsTableName, db));
    m_companiesDatabaseModel.reset(new SqlTableModel(CompaniesTableName, db));
    m_destinationsDatabaseModel.reset(new SqlTableModel(DestinationsTableName, db));

    // -------------------------------------------------------------

    m_eventsSourceModel.setSourceModel(m_eventsDatabaseModel.data());
    m_relationsSourceModel.setSourceModel(m_relationsDatabaseModel.data());
    m_unusedLotIdsModel.setSourceModel(m_unusedIdsDatabaseModel.data());

    m_producersSourceModel.setSourceModel(m_producersDatabaseModel.data());
    m_parcelsSourceModel.setSourceModel(m_parcelsDatabaseModel.data());
    m_companiesSourceModel.setSourceModel(m_companiesDatabaseModel.data());
    m_destinationsSourceModel.setSourceModel(m_destinationsDatabaseModel.data());

    // -------------------------------------------------------------

    m_producersViewModel.setSourceModel(new SqlQueryModel(ProducersTableName, db,
                                                          SortNameQuery(SortFilterQuery{}),
                                                          &m_producersViewModel));
    m_parcelsViewModel.setSourceModel(new SqlQueryModel(ParcelsTableName, db,
                                                        &m_producersViewModel));
    m_cooperativesViewModel.setSourceModel(new SqlQueryModel(CompaniesTableName, db,
                                                             SortNameQuery(CompanyTypeQuery(Enums::CompanyType::Cooperative, SortFilterQuery{})),
                                                             &m_cooperativesViewModel));
    m_buyersViewModel.setSourceModel(new SqlQueryModel(CompaniesTableName, db,
                                                       SortNameQuery(CompanyTypeQuery(Enums::CompanyType::Buyer, SortFilterQuery{})),
                                                       &m_buyersViewModel));
    m_transportersViewModel.setSourceModel(new SqlQueryModel(CompaniesTableName, db,
                                                             SortNameQuery(CompanyTypeQuery(Enums::CompanyType::Transporter, SortFilterQuery{})),
                                                             &m_transportersViewModel));
    m_destinationsViewModel.setSourceModel(new SqlQueryModel(DestinationsTableName, db,
                                                             SortNameQuery(SortFilterQuery{}),
                                                             &m_destinationsViewModel));

    // -------------------------------------------------------------

    m_cooperativeEventsModel.setSourceModel(&m_eventsSourceModel);
    m_lastActionHarvestModel.setSourceModel(&m_cooperativeEventsModel);
    m_lastActionGrainProcessingModel.setSourceModel(&m_cooperativeEventsModel);
    m_lastActionSectionReceptionModel.setSourceModel(&m_cooperativeEventsModel);
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

    // -------------------------------------------------------------

    setupUpdateConnections();
}

void DataModelsManager::updateCooperativeId(quint32 cooperativeId)
{
    m_cooperativeEventsModel.setCooperativeId(cooperativeId);
    m_cooperativeFilteringEventsModel.setCooperativeId(cooperativeId);
}

void DataModelsManager::addLocalAction(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                                       int cooperativeId, const QVariantMap &properties)
{
    ProcessCounter p(this);

    auto eventInfo = Gaia::ModelEntry{ packageId, QVariant::fromValue(action) };
    auto modelData = Gaia::ModelData { eventInfo, };
    removeExistingEvents(modelData);

    if (modelData.isEmpty()) {
        emit localActionDuplicated(packageId, action, timestamp, properties);
    } else {
        m_eventsSourceModel.appendData({ Gaia::ModelEntry {
                                             packageId,
                                             QVariant::fromValue(action),
                                             timestamp,
                                             cooperativeId,
                                             properties,
                                             0.0,    // location not handled yet
                                             0.0,    // location not handled yet
                                             true
                                         }, });

        emit localActionAdded(packageId, action, timestamp, properties);
    }
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

    auto modelData = Gaia::ModelData{};

    // producers
    if (modelsData.contains(Enums::AdditionalDataType::ProducersData)) {
        modelData = modelsData.value(Enums::AdditionalDataType::ProducersData);

        if (!m_producersSourceModel.clearData()) {
            removeExistingById(modelData, ProducersTableName);
        }
        m_producersSourceModel.appendData(modelData);
    }

    // parcels
    if (modelsData.contains(Enums::AdditionalDataType::ParcelsData)) {
        modelData = modelsData.value(Enums::AdditionalDataType::ParcelsData);

        if (!m_parcelsSourceModel.clearData()) {
            removeExistingById(modelData, ParcelsTableName);
        }
        m_parcelsSourceModel.appendData(modelData);
    }

    // companies
    if (modelsData.contains(Enums::AdditionalDataType::CompaniesData)) {
        modelData = modelsData.value(Enums::AdditionalDataType::CompaniesData);

        if (!m_companiesSourceModel.clearData()) {
            removeExistingById(modelData, CompaniesTableName);
        }
        m_companiesSourceModel.appendData(modelData);
    }

    // destinations
    if (modelsData.contains(Enums::AdditionalDataType::DestinationsData)) {
        modelData = modelsData.value(Enums::AdditionalDataType::DestinationsData);

        if (!m_destinationsSourceModel.clearData()) {
            removeExistingById(modelData, ParcelsTableName);
        }
        m_destinationsSourceModel.appendData(modelData);
    }
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
}

void DataModelsManager::processEntities(const Gaia::ModelData &modelData)
{
    ProcessCounter p(this);

    auto missingEvents = modelData;
    missingEvents.erase(std::remove_if(missingEvents.begin(), missingEvents.end(), &DataModelsManager::isInvalidAction),
                        missingEvents.end());

    removeExistingEvents(missingEvents);
    m_eventsSourceModel.appendData(missingEvents);
}

void DataModelsManager::processRelations(const Gaia::ModelData &modelData)
{
    ProcessCounter p(this);

    auto missingRelations = modelData;
    removeExistingRelations(missingRelations);
    m_relationsSourceModel.appendData(missingRelations);
}

void DataModelsManager::processUnusedLotIds(const Gaia::ModelData &modelData)
{
    ProcessCounter p(this);

    auto missingUnusedLotIds = modelData;
    if (!m_unusedLotIdsModel.clearData()) {
        removeExistingUnusedLotIds(missingUnusedLotIds);
    }
    m_unusedLotIdsModel.appendData(missingUnusedLotIds);
}

void DataModelsManager::getOfflineActions() const
{
    ProcessCounter p(this);

    emit offlineActions(m_localOnlyEventsModel.getData());
}

void DataModelsManager::setupUpdateConnections()
{
    connect(&m_producersSourceModel, &AbstractModel::modelChanged,
            this, std::bind(&DataModelsManager::scheduleModelUpdate,
                            this, qobject_cast<SqlQueryModel *>(m_producersViewModel.sourceModel())) );
    connect(&m_parcelsSourceModel, &AbstractModel::modelChanged,
            this, std::bind(&DataModelsManager::scheduleModelUpdate,
                            this, qobject_cast<SqlQueryModel *>(m_parcelsViewModel.sourceModel())) );
    connect(&m_companiesSourceModel, &AbstractModel::modelChanged,
            this, std::bind(&DataModelsManager::scheduleModelUpdate,
                            this, qobject_cast<SqlQueryModel *>(m_cooperativesViewModel.sourceModel())) );
    connect(&m_companiesSourceModel, &AbstractModel::modelChanged,
            this, std::bind(&DataModelsManager::scheduleModelUpdate,
                            this, qobject_cast<SqlQueryModel *>(m_buyersViewModel.sourceModel())) );
    connect(&m_companiesSourceModel, &AbstractModel::modelChanged,
            this, std::bind(&DataModelsManager::scheduleModelUpdate,
                            this, qobject_cast<SqlQueryModel *>(m_transportersViewModel.sourceModel())) );
    connect(&m_destinationsSourceModel, &AbstractModel::modelChanged,
            this, std::bind(&DataModelsManager::scheduleModelUpdate,
                            this, qobject_cast<SqlQueryModel *>(m_destinationsViewModel.sourceModel())) );

}

void DataModelsManager::scheduleModelUpdate(SqlQueryModel *model)
{
    if (!m_modelsToUpdate.contains(model)) {
        m_modelsToUpdate.append(QPointer(model));

        QMetaObject::invokeMethod(this, &DataModelsManager::updateModels, Qt::QueuedConnection);
    }
}

void DataModelsManager::updateModels()
{
    while (!m_modelsToUpdate.isEmpty()) {
        if (auto modelPtr = m_modelsToUpdate.takeFirst(); modelPtr) {
            modelPtr->select();
        }
    }
}

bool DataModelsManager::isInvalidAction(const Gaia::ModelEntry &entityEntry)
{
    Q_ASSERT(entityEntry.size() >= 2);
    return (entityEntry.at(1).value<Enums::SupplyChainAction>() == Enums::SupplyChainAction::Unknown);
}

void DataModelsManager::removeExistingById(Gaia::ModelData &modelData, const QLatin1String &tableName)
{
    static const auto IdColumnName = QLatin1String("id");
    static const auto ConditionsFields = QStringList{ IdColumnName, };

    m_existsQueryModel.prepareQuery(tableName, ConditionsFields);

    modelData.erase(std::remove_if(modelData.begin(), modelData.end(),
                                   [this](const auto &entry) {
        return entry.isEmpty() ? false
                               : m_existsQueryModel.exists({ entry.first() });
    }), modelData.end());
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

void DataModelsManager::removeExistingRelations(Gaia::ModelData &modelData)
{
    static const auto ConditionsFields = QStringList{
            m_relationsSourceModel.roleNames().value(RelationModel::Columns::PackageId),
            m_relationsSourceModel.roleNames().value(RelationModel::Columns::RelatedId) };

    m_existsQueryModel.prepareQuery(RelationsTableName, ConditionsFields);

    modelData.erase(std::remove_if(modelData.begin(), modelData.end(),
                                   [this](const auto &entry) {
        return entry.size() < 2 ? false
                                : m_existsQueryModel.exists({ entry.at(0), entry.at(1) });
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
