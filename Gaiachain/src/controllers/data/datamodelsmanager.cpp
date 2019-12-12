#include "datamodelsmanager.h"

#include "../../database/dbhelpers.h"
#include "../../common/dataglobals.h"

DataModelsManager::DataModelsManager(QObject *parent)
    : AbstractDataModelsManager(parent)
{}

void DataModelsManager::updateThread()
{
    m_existsQueryModel.moveToThread(thread());

    m_producersSourceModel.moveToThread(thread());
    m_parcelsSourceModel.moveToThread(thread());
    m_companiesSourceModel.moveToThread(thread());
    m_destinationsSourceModel.moveToThread(thread());

    m_eventsSourceModel.moveToThread(thread());
    m_relationsSourceModel.moveToThread(thread());
    m_unusedLotIdsSourceModel.moveToThread(thread());
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

void DataModelsManager::processPackageData(const QString &packageId, const Enums::PackageType &packageType)
{
    auto eventsInfo = allPackageEvents(packageId, packageType);

    processEntitiesInfo(eventsInfo);
}

void DataModelsManager::processPackagesInfo(const Gaia::ModelData &modelData)
{
    auto eventsInfo = Gaia::ModelData{};

    std::for_each(modelData.constBegin(), modelData.constEnd(),
                  [&eventsInfo](const auto &eventInfo) {
        Q_ASSERT(eventInfo.size() >= 2);

        const auto packageType = DataGlobals::packageType(eventInfo.at(1).template value<Enums::SupplyChainAction>());
        eventsInfo.append(allPackageEvents(eventInfo.at(0).toString(), packageType));
    });

    processEntitiesInfo(eventsInfo);
}

void DataModelsManager::processAdditionalData(const QMap<Enums::AdditionalDataType, Gaia::ModelData> &modelsData)
{
    ProcessCounter p(this);

    auto modelData = Gaia::ModelData{};

    // producers
    if (modelsData.contains(Enums::AdditionalDataType::ProducersData)) {
        modelData = modelsData.value(Enums::AdditionalDataType::ProducersData);

        if (!m_producersSourceModel.clearData()) {
            removeExistingById(modelData, sc_databaseTableName.value(ModelType::Producers));
        }
        m_producersSourceModel.appendData(modelData);
    }

    // parcels
    if (modelsData.contains(Enums::AdditionalDataType::ParcelsData)) {
        modelData = modelsData.value(Enums::AdditionalDataType::ParcelsData);

        if (!m_parcelsSourceModel.clearData()) {
            removeExistingById(modelData, sc_databaseTableName.value(ModelType::Parcels));
        }
        m_parcelsSourceModel.appendData(modelData);
    }

    // companies
    if (modelsData.contains(Enums::AdditionalDataType::CompaniesData)) {
        modelData = modelsData.value(Enums::AdditionalDataType::CompaniesData);

        if (!m_companiesSourceModel.clearData()) {
            removeExistingById(modelData, sc_databaseTableName.value(ModelType::Companies));
        }
        m_companiesSourceModel.appendData(modelData);
    }

    // destinations
    if (modelsData.contains(Enums::AdditionalDataType::DestinationsData)) {
        modelData = modelsData.value(Enums::AdditionalDataType::DestinationsData);

        if (!m_destinationsSourceModel.clearData()) {
            removeExistingById(modelData, sc_databaseTableName.value(ModelType::Destinations));
        }
        m_destinationsSourceModel.appendData(modelData);
    }
}

void DataModelsManager::processEntitiesInfo(const Gaia::ModelData &modelData)
{
    ProcessCounter p(this);

    // events
    auto missingEventsInfo = modelData;
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
    if (!m_unusedLotIdsSourceModel.clearData()) {
        removeExistingUnusedLotIds(missingUnusedLotIds);
    }
    m_unusedLotIdsSourceModel.appendData(missingUnusedLotIds);
}

void DataModelsManager::setupModels()
{
    m_existsQueryModel.setDatabase(m_db);

    // -------------------------------------------------------------

    m_producersSourceModel.setSourceModel(new SqlTableModel(sc_databaseTableName.value(ModelType::Producers),
                                                            m_db, &m_producersSourceModel));
    m_parcelsSourceModel.setSourceModel(new SqlTableModel(sc_databaseTableName.value(ModelType::Parcels),
                                                          m_db, &m_parcelsSourceModel));
    m_companiesSourceModel.setSourceModel(new SqlTableModel(sc_databaseTableName.value(ModelType::Companies),
                                                            m_db, &m_companiesSourceModel));
    m_destinationsSourceModel.setSourceModel(new SqlTableModel(sc_databaseTableName.value(ModelType::Destinations),
                                                               m_db, &m_destinationsSourceModel));

    m_eventsSourceModel.setSourceModel(new SqlTableModel(sc_databaseTableName.value(ModelType::Events),
                                                         m_db, &m_eventsSourceModel));
    m_relationsSourceModel.setSourceModel(new SqlTableModel(sc_databaseTableName.value(ModelType::Relations),
                                                            m_db, &m_relationsSourceModel));
    m_unusedLotIdsSourceModel.setSourceModel(new SqlTableModel(sc_databaseTableName.value(ModelType::UnusedIds),
                                                               m_db, &m_unusedLotIdsSourceModel));
}

void DataModelsManager::setupUpdateConnections()
{
    connect(&m_eventsSourceModel, &AbstractModel::entryInserted, this, &DataModelsManager::eventInserted);
    connect(&m_relationsSourceModel, &AbstractModel::entryInserted, this, &DataModelsManager::relationInserted);

    connect(&m_producersSourceModel, &AbstractModel::modelChanged,
            this, std::bind(&DataModelsManager::modelUpdated, this, ModelType::Producers));
    connect(&m_parcelsSourceModel, &AbstractModel::modelChanged,
            this, std::bind(&DataModelsManager::modelUpdated, this, ModelType::Parcels));
    connect(&m_companiesSourceModel, &AbstractModel::modelChanged,
            this, std::bind(&DataModelsManager::modelUpdated, this, ModelType::Companies));
    connect(&m_destinationsSourceModel, &AbstractModel::modelChanged,
            this, std::bind(&DataModelsManager::modelUpdated, this, ModelType::Destinations));

    connect(&m_eventsSourceModel, &AbstractModel::modelChanged,
            this, std::bind(&DataModelsManager::modelUpdated, this, ModelType::Events));
    connect(&m_relationsSourceModel, &AbstractModel::modelChanged,
            this, std::bind(&DataModelsManager::modelUpdated, this, ModelType::Relations));
    connect(&m_unusedLotIdsSourceModel, &AbstractModel::modelChanged,
            this, std::bind(&DataModelsManager::modelUpdated, this, ModelType::UnusedIds));
}

bool DataModelsManager::isInvalidAction(const Gaia::ModelEntry &entityEntry)
{
    Q_ASSERT(entityEntry.size() >= 2);
    return (entityEntry.at(1).value<Enums::SupplyChainAction>() == Enums::SupplyChainAction::Unknown);
}

Gaia::ModelData DataModelsManager::allPackageEvents(const QString &packageId, const Enums::PackageType &packageType)
{
    auto eventsInfo = Gaia::ModelData{};
    const auto packageActions = DataGlobals::packageActions(packageType);

    std::transform(packageActions.constBegin(), packageActions.constEnd(), std::back_inserter(eventsInfo),
                   [&packageId](const auto &action) { return Gaia::ModelEntry{ packageId, QVariant::fromValue(action) }; });

    return eventsInfo;
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

    m_existsQueryModel.prepareQuery(sc_databaseTableName.value(ModelType::Events), ConditionsFields);

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

    m_existsQueryModel.prepareQuery(sc_databaseTableName.value(ModelType::Relations), ConditionsFields);

    modelData.erase(std::remove_if(modelData.begin(), modelData.end(),
                                   [this](const auto &entry) {
        return entry.size() < 2 ? false
                                : m_existsQueryModel.exists({ entry.at(0), entry.at(1) });
    }), modelData.end());
}

void DataModelsManager::removeExistingUnusedLotIds(Gaia::ModelData &modelData)
{
    static const auto ConditionsFields = QStringList{
            m_unusedLotIdsSourceModel.roleNames().value(UnusedIdsModel::Columns::PackageId) };

    m_existsQueryModel.prepareQuery(sc_databaseTableName.value(ModelType::UnusedIds), ConditionsFields);

    modelData.erase(std::remove_if(modelData.begin(), modelData.end(),
                                   [this](const auto &entry) {
        return entry.isEmpty() ? false
                               : m_existsQueryModel.exists({ entry.first() });
    }), modelData.end());
}
