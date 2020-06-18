#include "dataviewmanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDateTime>

#include "../../models/query/sqlquerymodel.h"
#include "../../models/query/companytypequery.h"
#include "../../models/query/sortnamequery.h"

#include "database/dbhelpers.h"

namespace {
const std::chrono::milliseconds updateDelay = std::chrono::milliseconds(500);
}

DataViewManager::DataViewManager(QObject *parent)
    : AbstractDataModelsManager(parent)
{
    m_updateTimer.setSingleShot(true);
    connect(&m_updateTimer, &QTimer::timeout, this, &DataViewManager::updateModels);

    connect(&m_latestRangeTransactionsModel, &LatestRangeEventsProxyModel::fetchEvents,
            this, [this](int number, int offset) {
        const auto keyword = m_transactionsViewModel.keyword();
        const auto activePackageTypes = m_transactionsViewModel.activePackageTypes();
        const auto cooperativeId = m_transactionsViewModel.filterCooperativeId();
        emit limitKeywordEventsNeeded(number, offset, keyword, activePackageTypes, cooperativeId);
    });
    connect(&m_latestRangeDateModel, &LatestRangeEventsProxyModel::fetchEvents,
            this, [this](int number, int offset) {
        emit limitRangeEventsNeeded(number, offset,
                                    m_calendarDateViewModel.fromDateTime(), m_calendarDateViewModel.toDateTime());
    });
}

void DataViewManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("producersModel"), &m_producersViewModel);
    engine.rootContext()->setContextProperty(QStringLiteral("parcelsModel"), &m_parcelsViewModel);
    engine.rootContext()->setContextProperty(QStringLiteral("cooperativesModel"), &m_cooperativesViewModel);
    engine.rootContext()->setContextProperty(QStringLiteral("buyersModel"), &m_buyersViewModel);
    engine.rootContext()->setContextProperty(QStringLiteral("transportersModel"), &m_transportersViewModel);
    engine.rootContext()->setContextProperty(QStringLiteral("destinationsModel"), &m_destinationsViewModel);

    engine.rootContext()->setContextProperty(QStringLiteral("transactionsModel"), &m_transactionsViewModel);
    engine.rootContext()->setContextProperty(QStringLiteral("latestRangeTransactionsModel"), &m_latestRangeTransactionsModel);

    engine.rootContext()->setContextProperty(QStringLiteral("calendarMonthModel"), &m_calendarMonthViewModel);
    engine.rootContext()->setContextProperty(QStringLiteral("packagesCalendarMonthModel"), &m_packagesCalendarMonthModel);

    engine.rootContext()->setContextProperty(QStringLiteral("calendarDateModel"), &m_calendarDateViewModel);
    engine.rootContext()->setContextProperty(QStringLiteral("latestRangeDateModel"), &m_latestRangeDateModel);

    engine.rootContext()->setContextProperty(QStringLiteral("localEventsModel"), &m_localEventsViewModel);

    engine.rootContext()->setContextProperty(QStringLiteral("unusedLotIdsModel"), &m_unusedLotIdsViewModel);

    engine.rootContext()->setContextProperty(QStringLiteral("lastActionHarvestModel"), &m_lastActionHarvestModel);
    engine.rootContext()->setContextProperty(QStringLiteral("lastActionGrainProcessingModel"), &m_lastActionGrainProcessingModel);
    engine.rootContext()->setContextProperty(QStringLiteral("lastActionSectionReceptionModel"), &m_lastActionSectionReceptionModel);
}

void DataViewManager::updateCooperativeId(quint32 cooperativeId)
{
    m_transactionsViewModel.setCooperativeId(cooperativeId);
    m_calendarMonthViewModel.setCooperativeId(cooperativeId);
    m_calendarDateViewModel.setCooperativeId(cooperativeId);
    m_cooperativeEventsViewModel.setCooperativeId(cooperativeId);
    m_localEventsViewModel.setCooperativeId(cooperativeId);
}

PackageData DataViewManager::preparePackageData(const QString &packageId)
{
    auto package = PackageData{};
    package.id = packageId;

    m_packageRelationsViewModel.setPackageId(packageId);
    package.relatedPackages = m_packageRelationsViewModel.relatedPackages();

    m_packageViewModel.setPackageId(packageId);
    m_packageViewModel.fillPackageData(package);

    return package;
}

Gaia::ModelData DataViewManager::getOfflineActions() const
{
    return m_localEventsViewModel.getData();
}

void DataViewManager::onModelUpdated(const ModelType &type)
{
    m_modelUpdateHandler.value(type, []{})();
}

void DataViewManager::setupModels()
{
    m_producersViewModel.setSourceModel(new SqlQueryModel(sc_databaseTableName.value(ModelType::Producers), c_dbConnectionName,
                                                          SortNameQuery(SortFilterQuery{}),
                                                          &m_producersViewModel));
    m_parcelsViewModel.setSourceModel(new SqlQueryModel(sc_databaseTableName.value(ModelType::Parcels), c_dbConnectionName,
                                                        &m_producersViewModel));
    m_cooperativesViewModel.setSourceModel(new SqlQueryModel(sc_databaseTableName.value(ModelType::Companies), c_dbConnectionName,
                                                             SortNameQuery(CompanyTypeQuery(Enums::CompanyType::Cooperative, SortFilterQuery{})),
                                                             &m_cooperativesViewModel));
    m_buyersViewModel.setSourceModel(new SqlQueryModel(sc_databaseTableName.value(ModelType::Companies), c_dbConnectionName,
                                                       SortNameQuery(CompanyTypeQuery(Enums::CompanyType::Buyer, SortFilterQuery{})),
                                                       &m_buyersViewModel));
    m_transportersViewModel.setSourceModel(new SqlQueryModel(sc_databaseTableName.value(ModelType::Companies), c_dbConnectionName,
                                                             SortNameQuery(CompanyTypeQuery(Enums::CompanyType::Transporter, SortFilterQuery{})),
                                                             &m_transportersViewModel));
    m_destinationsViewModel.setSourceModel(new SqlQueryModel(sc_databaseTableName.value(ModelType::Destinations), c_dbConnectionName,
                                                             SortNameQuery(SortFilterQuery{}),
                                                             &m_destinationsViewModel));

    m_transactionsViewModel.setSourceModel(new SqlQueryModel(sc_databaseTableName.value(ModelType::Events), c_dbConnectionName,
                                                             &m_transactionsViewModel));
    m_calendarMonthViewModel.setSourceModel(new SqlQueryModel(sc_databaseTableName.value(ModelType::Events), c_dbConnectionName,
                                                              &m_calendarMonthViewModel));
    m_calendarDateViewModel.setSourceModel(new SqlQueryModel(sc_databaseTableName.value(ModelType::Events), c_dbConnectionName,
                                                             &m_calendarDateViewModel));

    m_cooperativeEventsViewModel.setSourceModel(new SqlQueryModel(sc_databaseTableName.value(ModelType::Events), c_dbConnectionName,
                                                                  &m_cooperativeEventsViewModel));
    m_localEventsViewModel.setSourceModel(new SqlQueryModel(sc_databaseTableName.value(ModelType::Events), c_dbConnectionName,
                                                            &m_localEventsViewModel));

    m_packageViewModel.setSourceModel(new SqlQueryModel(sc_databaseTableName.value(ModelType::Events), c_dbConnectionName,
                                                        SortFilterQuery{},
                                                        &m_packageViewModel));
    m_packageRelationsViewModel.setSourceModel(new SqlQueryModel(sc_databaseTableName.value(ModelType::Relations), c_dbConnectionName,
                                                                 SortFilterQuery{},
                                                                 &m_packageRelationsViewModel));

    m_unusedLotIdsViewModel.setSourceModel(new SqlQueryModel(sc_databaseTableName.value(ModelType::UnusedIds), c_dbConnectionName,
                                                             SortFilterQuery{},
                                                             &m_unusedLotIdsViewModel));
    // -------------------------------------------------------------

    m_latestRangeTransactionsModel.setSourceModel(&m_transactionsViewModel);
    m_packagesCalendarMonthModel.setSourceModel(&m_calendarMonthViewModel);
    m_latestRangeDateModel.setSourceModel(&m_calendarDateViewModel);

    m_lastActionHarvestModel.setSourceModel(&m_cooperativeEventsViewModel);
    m_lastActionGrainProcessingModel.setSourceModel(&m_cooperativeEventsViewModel);
    m_lastActionSectionReceptionModel.setSourceModel(&m_cooperativeEventsViewModel);
}

void DataViewManager::setupUpdateConnections()
{
    connect(&m_lastActionHarvestModel, &PackageLastActionProxyModel::filteringFinished,
            this, [this]() { packagesEventsNeeded(m_lastActionHarvestModel.getData()); });
    connect(&m_lastActionGrainProcessingModel, &PackageLastActionProxyModel::filteringFinished,
            this, [this]() { packagesEventsNeeded(m_lastActionGrainProcessingModel.getData()); });
    connect(&m_lastActionSectionReceptionModel, &PackageLastActionProxyModel::filteringFinished,
            this, [this]() { packagesEventsNeeded(m_lastActionSectionReceptionModel.getData()); });

    // -------------------------------------------------------------

    m_modelUpdateHandler.insert(ModelType::Producers,
                                std::bind(&DataViewManager::scheduleModelUpdate, this,
                                          qobject_cast<SqlQueryModel *>(m_producersViewModel.sourceModel()) ));
    m_modelUpdateHandler.insert(ModelType::Parcels,
                                std::bind(&DataViewManager::scheduleModelUpdate, this,
                                          qobject_cast<SqlQueryModel *>(m_parcelsViewModel.sourceModel()) ));
    m_modelUpdateHandler.insert(ModelType::Companies, [this]() {
        scheduleModelUpdate(qobject_cast<SqlQueryModel *>(m_cooperativesViewModel.sourceModel()));
        scheduleModelUpdate(qobject_cast<SqlQueryModel *>(m_buyersViewModel.sourceModel()));
        scheduleModelUpdate(qobject_cast<SqlQueryModel *>(m_transportersViewModel.sourceModel()));
    });
    m_modelUpdateHandler.insert(ModelType::Destinations,
                                std::bind(&DataViewManager::scheduleModelUpdate, this,
                                          qobject_cast<SqlQueryModel *>(m_destinationsViewModel.sourceModel()) ));


    m_modelUpdateHandler.insert(ModelType::Events, [this]() {
        scheduleModelUpdate(qobject_cast<SqlQueryModel *>(m_transactionsViewModel.sourceModel()));
        scheduleModelUpdate(qobject_cast<SqlQueryModel *>(m_calendarMonthViewModel.sourceModel()));
        scheduleModelUpdate(qobject_cast<SqlQueryModel *>(m_calendarDateViewModel.sourceModel()));
        scheduleModelUpdate(qobject_cast<SqlQueryModel *>(m_cooperativeEventsViewModel.sourceModel()));
        scheduleModelUpdate(qobject_cast<SqlQueryModel *>(m_localEventsViewModel.sourceModel()));
        scheduleModelUpdate(qobject_cast<SqlQueryModel *>(m_packageViewModel.sourceModel()));
    });
    m_modelUpdateHandler.insert(ModelType::Relations,
                                std::bind(&DataViewManager::scheduleModelUpdate, this,
                                          qobject_cast<SqlQueryModel *>(m_packageRelationsViewModel.sourceModel()) ));
    m_modelUpdateHandler.insert(ModelType::UnusedIds,
                                std::bind(&DataViewManager::scheduleModelUpdate, this,
                                          qobject_cast<SqlQueryModel *>(m_unusedLotIdsViewModel.sourceModel()) ));
}

void DataViewManager::scheduleModelUpdate(SqlQueryModel *model)
{
    Q_ASSERT(model);
    if (model) {
        if (!m_modelsToUpdate.contains(model)) {
            m_modelsToUpdate.append(QPointer(model));

            m_updateTimer.start(updateDelay);
        }
    }
}

void DataViewManager::updateModels()
{
    while (!m_modelsToUpdate.isEmpty()) {
        if (auto modelPtr = m_modelsToUpdate.takeFirst(); modelPtr) {
            modelPtr->select();
        }
    }
}
