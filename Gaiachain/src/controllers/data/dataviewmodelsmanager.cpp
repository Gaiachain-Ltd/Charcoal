#include "dataviewmodelsmanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDateTime>

#include "../../models/proxy/sqlquerymodel.h"
#include "../../models/proxy/companytypequery.h"
#include "../../models/proxy/sortnamequery.h"

#include "../../database/dbhelpers.h"

namespace {
const std::chrono::milliseconds updateDelay = std::chrono::milliseconds(500);
}

DataViewModelsManager::DataViewModelsManager(QObject *parent)
    : AbstractDataModelsManager(parent)
{
    m_updateTimer.setSingleShot(true);
    connect(&m_updateTimer, &QTimer::timeout, this, &DataViewModelsManager::updateModels);

    connect(&m_latestRangePackagesTypeSearchEventsModel, &LatestRangeEventsProxyModel::fetchEvents,
            this, [this](int number, int offset) { emit limitKeywordEventsNeeded(number, offset, m_searchEventsModel.keyword()); });
    connect(&m_latestRangeDateEventsModel, &LatestRangeEventsProxyModel::fetchEvents,
            this, [this](int number, int offset) {
        emit limitRangeEventsNeeded(number, offset,
                                  m_dateEventsModel.startDateTime(), m_dateEventsModel.endDateTime());
    });
}

void DataViewModelsManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("producersModel"), &m_producersViewModel);
    engine.rootContext()->setContextProperty(QStringLiteral("parcelsModel"), &m_parcelsViewModel);
    engine.rootContext()->setContextProperty(QStringLiteral("cooperativesModel"), &m_cooperativesViewModel);
    engine.rootContext()->setContextProperty(QStringLiteral("buyersModel"), &m_buyersViewModel);
    engine.rootContext()->setContextProperty(QStringLiteral("transportersModel"), &m_transportersViewModel);
    engine.rootContext()->setContextProperty(QStringLiteral("destinationsModel"), &m_destinationsViewModel);

    engine.rootContext()->setContextProperty(QStringLiteral("unusedLotIdsModel"), &m_unusedLotIdsViewModel);

    engine.rootContext()->setContextProperty(QStringLiteral("localOnlyEventsModel"), &m_localOnlyEventsModel);

    engine.rootContext()->setContextProperty(QStringLiteral("lastActionHarvestModel"), &m_lastActionHarvestModel);
    engine.rootContext()->setContextProperty(QStringLiteral("lastActionGrainProcessingModel"), &m_lastActionGrainProcessingModel);
    engine.rootContext()->setContextProperty(QStringLiteral("lastActionSectionReceptionModel"), &m_lastActionSectionReceptionModel);
    engine.rootContext()->setContextProperty(QStringLiteral("packageTypeCooperativeIdsModel"), &m_packageTypeCooperativeIdsModel);

    engine.rootContext()->setContextProperty(QStringLiteral("cooperativeFilteringEvents"), &m_cooperativeFilteringEventsModel);

    engine.rootContext()->setContextProperty(QStringLiteral("calendarModel"), &m_calendarModel);
    engine.rootContext()->setContextProperty(QStringLiteral("packagesCalendarModel"), &m_packagesCalendarModel);

    engine.rootContext()->setContextProperty(QStringLiteral("dateEventsModel"), &m_dateEventsModel);
    engine.rootContext()->setContextProperty(QStringLiteral("latestRangeDateEventsModel"), &m_latestRangeDateEventsModel);

    engine.rootContext()->setContextProperty(QStringLiteral("searchEventsModel"), &m_searchEventsModel);
    engine.rootContext()->setContextProperty(QStringLiteral("packagesTypeSearchEventsModel"), &m_packagesTypeSearchEventsModel);
    engine.rootContext()->setContextProperty(QStringLiteral("latestRangePackagesTypeSearchEventsModel"), &m_latestRangePackagesTypeSearchEventsModel);
}

void DataViewModelsManager::updateCooperativeId(quint32 cooperativeId)
{
    m_cooperativeEventsModel.setCooperativeId(cooperativeId);
    m_cooperativeFilteringEventsModel.setCooperativeId(cooperativeId);
}

PackageData DataViewModelsManager::getPackageData(const QString &packageId) const
{
    auto package = PackageData{};
    package.id = packageId;
    package.relatedPackages = m_relationsListModel.relatedPackages(packageId);
    m_packageDataModel.fillPackageData(package);

    return package;
}

Gaia::ModelData DataViewModelsManager::getOfflineActions() const
{
    return m_localOnlyEventsModel.getData();
}

void DataViewModelsManager::onModelUpdated(const ModelType &type)
{
    m_modelUpdateHandler.value(type, []{})();
}

void DataViewModelsManager::setupModels()
{
    m_producersViewModel.setSourceModel(new SqlQueryModel(sc_databaseTableName.value(ModelType::Producers), m_db,
                                                          SortNameQuery(SortFilterQuery{}),
                                                          &m_producersViewModel));
    m_parcelsViewModel.setSourceModel(new SqlQueryModel(sc_databaseTableName.value(ModelType::Parcels), m_db,
                                                        &m_producersViewModel));
    m_cooperativesViewModel.setSourceModel(new SqlQueryModel(sc_databaseTableName.value(ModelType::Companies), m_db,
                                                             SortNameQuery(CompanyTypeQuery(Enums::CompanyType::Cooperative, SortFilterQuery{})),
                                                             &m_cooperativesViewModel));
    m_buyersViewModel.setSourceModel(new SqlQueryModel(sc_databaseTableName.value(ModelType::Companies), m_db,
                                                       SortNameQuery(CompanyTypeQuery(Enums::CompanyType::Buyer, SortFilterQuery{})),
                                                       &m_buyersViewModel));
    m_transportersViewModel.setSourceModel(new SqlQueryModel(sc_databaseTableName.value(ModelType::Companies), m_db,
                                                             SortNameQuery(CompanyTypeQuery(Enums::CompanyType::Transporter, SortFilterQuery{})),
                                                             &m_transportersViewModel));
    m_destinationsViewModel.setSourceModel(new SqlQueryModel(sc_databaseTableName.value(ModelType::Destinations), m_db,
                                                             SortNameQuery(SortFilterQuery{}),
                                                             &m_destinationsViewModel));

    m_eventsViewModel.setSourceModel(new SqlQueryModel(sc_databaseTableName.value(ModelType::Events), m_db,
                                                       SortFilterQuery{},
                                                       &m_eventsViewModel));
    m_relationsViewModel.setSourceModel(new SqlQueryModel(sc_databaseTableName.value(ModelType::Relations), m_db,
                                                          SortFilterQuery{},
                                                          &m_relationsViewModel));
    m_unusedLotIdsViewModel.setSourceModel(new SqlQueryModel(sc_databaseTableName.value(ModelType::UnusedIds), m_db,
                                                             SortFilterQuery{},
                                                             &m_unusedLotIdsViewModel));

    // -------------------------------------------------------------

    m_cooperativeEventsModel.setSourceModel(&m_eventsViewModel);
    m_lastActionHarvestModel.setSourceModel(&m_cooperativeEventsModel);
    m_lastActionGrainProcessingModel.setSourceModel(&m_cooperativeEventsModel);
    m_lastActionSectionReceptionModel.setSourceModel(&m_cooperativeEventsModel);
    m_packageTypeCooperativeIdsModel.setSourceModel(&m_cooperativeEventsModel);

    m_localOnlyEventsModel.setSourceModel(&m_cooperativeEventsModel);

    m_cooperativeFilteringEventsModel.setSourceModel(&m_eventsViewModel);
    m_calendarModel.setSourceModel(&m_cooperativeFilteringEventsModel);
    m_packagesCalendarModel.setSourceModel(&m_calendarModel);

    m_dateEventsModel.setSourceModel(&m_calendarModel);
    m_latestRangeDateEventsModel.setSourceModel(&m_dateEventsModel);

    m_searchEventsModel.setSourceModel(&m_cooperativeFilteringEventsModel);
    m_packagesTypeSearchEventsModel.setSourceModel(&m_searchEventsModel);
    m_latestRangePackagesTypeSearchEventsModel.setSourceModel(&m_packagesTypeSearchEventsModel);

    m_packageDataModel.setSourceModel(&m_eventsViewModel);
    m_relationsListModel.setSourceModel(&m_relationsViewModel);
}

void DataViewModelsManager::setupUpdateConnections()
{
    connect(&m_lastActionHarvestModel, &PackageLastActionProxyModel::filteringFinished,
            this, [this]() { packagesEventsNeeded(m_lastActionHarvestModel.getData()); });
    connect(&m_lastActionGrainProcessingModel, &PackageLastActionProxyModel::filteringFinished,
            this, [this]() { packagesEventsNeeded(m_lastActionGrainProcessingModel.getData()); });
    connect(&m_lastActionSectionReceptionModel, &PackageLastActionProxyModel::filteringFinished,
            this, [this]() { packagesEventsNeeded(m_lastActionSectionReceptionModel.getData()); });

    // -------------------------------------------------------------

    m_modelUpdateHandler.insert(ModelType::Producers,
                                std::bind(&DataViewModelsManager::scheduleModelUpdate, this,
                                          dynamic_cast<SqlQueryModel *>(m_producersViewModel.sourceModel()) ));
    m_modelUpdateHandler.insert(ModelType::Parcels,
                                std::bind(&DataViewModelsManager::scheduleModelUpdate, this,
                                          dynamic_cast<SqlQueryModel *>(m_parcelsViewModel.sourceModel()) ));
    m_modelUpdateHandler.insert(ModelType::Companies,
                                std::bind(&DataViewModelsManager::scheduleModelUpdate, this,
                                          dynamic_cast<SqlQueryModel *>(m_cooperativesViewModel.sourceModel()) ));
    m_modelUpdateHandler.insert(ModelType::Companies,
                                std::bind(&DataViewModelsManager::scheduleModelUpdate, this,
                                          dynamic_cast<SqlQueryModel *>(m_buyersViewModel.sourceModel()) ));
    m_modelUpdateHandler.insert(ModelType::Companies,
                                std::bind(&DataViewModelsManager::scheduleModelUpdate, this,
                                          dynamic_cast<SqlQueryModel *>(m_transportersViewModel.sourceModel()) ));
    m_modelUpdateHandler.insert(ModelType::Destinations,
                                std::bind(&DataViewModelsManager::scheduleModelUpdate, this,
                                          dynamic_cast<SqlQueryModel *>(m_destinationsViewModel.sourceModel()) ));

    m_modelUpdateHandler.insert(ModelType::Events,
                                std::bind(&DataViewModelsManager::scheduleModelUpdate, this,
                                          dynamic_cast<SqlQueryModel *>(m_eventsViewModel.sourceModel()) ));
    m_modelUpdateHandler.insert(ModelType::Relations,
                                std::bind(&DataViewModelsManager::scheduleModelUpdate, this,
                                          dynamic_cast<SqlQueryModel *>(m_relationsViewModel.sourceModel()) ));
    m_modelUpdateHandler.insert(ModelType::UnusedIds,
                                std::bind(&DataViewModelsManager::scheduleModelUpdate, this,
                                          dynamic_cast<SqlQueryModel *>(m_unusedLotIdsViewModel.sourceModel()) ));
}

void DataViewModelsManager::scheduleModelUpdate(SqlQueryModel *model)
{
    Q_ASSERT(model);
    if (model) {
        if (!m_modelsToUpdate.contains(model)) {
            m_modelsToUpdate.append(QPointer(model));

            m_updateTimer.start(updateDelay);
        }
    }
}

void DataViewModelsManager::updateModels()
{
    while (!m_modelsToUpdate.isEmpty()) {
        if (auto modelPtr = m_modelsToUpdate.takeFirst(); modelPtr) {
            modelPtr->select();
        }
    }
}
