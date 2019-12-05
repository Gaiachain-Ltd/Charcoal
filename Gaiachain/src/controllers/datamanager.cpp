#include "datamanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QJsonObject>
#include <QJsonArray>

#include "../common/dataglobals.h"
#include "../helpers/packagedataproperties.h"

#include <QLoggingCategory>
Q_LOGGING_CATEGORY(dataManager, "data.manager")

DataManager::DataManager(QObject *parent)
    : AbstractManager(parent)
{
    m_processingThread.start();
    m_modelsHandler.moveToThread(&m_processingThread);
    m_requestsHandler.moveToThread(&m_processingThread);

    setupHandlersConnections();
}

DataManager::~DataManager()
{
    m_processingThread.quit();
    m_processingThread.wait();
}

void DataManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("dataManager"), this);

    m_modelsHandler.setupQmlContext(engine);
}

void DataManager::setupModels(QSqlDatabase db)
{
    m_modelsHandler.setupModels(db);
}

void DataManager::updateUserData(const UserData &userData)
{
    m_userData = userData;
    QMetaObject::invokeMethod(&m_modelsHandler, std::bind(&DataModelsManager::updateCooperativeId, &m_modelsHandler, userData.cooperativeId));
}

bool DataManager::processing() const
{
    return (m_modelsHandler.processing() || m_requestsHandler.processing());
}

void DataManager::getPackageData(const QString &packageId) const
{
    QMetaObject::invokeMethod(const_cast<DataModelsManager *>(&m_modelsHandler),    // const cast required to use invokeMethod. Method called is const.
                              std::bind(&DataModelsManager::getPackageData, &m_modelsHandler,
                                                          packageId));
}

void DataManager::addHarvestAction(const QString &parcelCode, const QDateTime &timestamp, const QVariantMap &properties)
{
    auto id = generateHarvestId(properties.value(PackageDataProperties::HarvestDate).toDate(), parcelCode);
    addAction(id, Enums::SupplyChainAction::Harvest, timestamp, properties);
}

void DataManager::addAction(const QString &packageId, const Enums::SupplyChainAction &action,
                            const QDateTime &timestamp, const QVariantMap &properties)
{
    auto isOfflineAction = DataGlobals::availableOfflineActions().contains(action) && !m_userData.isAnonymous();
    if (isOfflineAction) {
        QMetaObject::invokeMethod(&m_modelsHandler, std::bind(&DataModelsManager::addLocalAction, &m_modelsHandler,
                                                              packageId, action, timestamp, m_userData.cooperativeId, properties));
        QMetaObject::invokeMethod(&m_requestsHandler, std::bind(&DataRequestsManager::processOfflineAction, &m_requestsHandler,
                                                                packageId, action));
    }

    emit addActionRequest(packageId, action, timestamp, properties);
}

void DataManager::addAction(const QString &packageId, const QByteArray &codeData, const Enums::SupplyChainAction &action,
                            const QDateTime &timestamp, const QVariantMap &properties)
{
    emit addActionRequest(packageId, codeData, action, timestamp, properties);
}

void DataManager::addAction(const QByteArray &codeData, const Enums::SupplyChainAction &action,
                            const QDateTime &timestamp, const QVariantMap &properties)
{
    emit addActionRequest(codeData, action, timestamp, properties);
}

void DataManager::sendOfflineActions()
{
    QMetaObject::invokeMethod(const_cast<DataModelsManager *>(&m_modelsHandler),    // const cast required to use invokeMethod. Method called is const.
                              std::bind(&DataModelsManager::getOfflineActions, &m_modelsHandler));
}

void DataManager::fetchEventData(const QString &packageId, const Enums::PackageType &type)
{
    auto eventsInfo = Gaia::ModelData{};

    const auto packageActions = DataGlobals::packageActions(type);
    std::transform(packageActions.constBegin(), packageActions.constEnd(), std::back_inserter(eventsInfo),
                   [&packageId](const auto &action) { return Gaia::ModelEntry{ packageId, QVariant::fromValue(action) }; });

    QMetaObject::invokeMethod(&m_modelsHandler, std::bind(&DataModelsManager::processEntitiesInfo, &m_modelsHandler,
                                                          eventsInfo));
}

void DataManager::fetchRangeEvents(const QDateTime &from, const QDateTime &to, const QString &keyword)
{
    emit eventsInfoNeeded(from, to, keyword);
}

void DataManager::fetchLimitEvents(int limit, const QDateTime &from, const QString &keyword)
{
    emit eventsInfoNeeded(limit, from, keyword);
}

void DataManager::fetchLastActionPackageEvents(const Enums::SupplyChainAction &lastAction)
{
    emit lastActionEventsInfoNeeded(lastAction);
}

void DataManager::onActionAdded(const QString &packageId, const Enums::SupplyChainAction &action)
{
    QMetaObject::invokeMethod(&m_requestsHandler, std::bind(&DataRequestsManager::offlineActionAdded, &m_requestsHandler,
                                                            packageId, action));
}

void DataManager::onActionAddError(const QString &packageId, const Enums::SupplyChainAction &action, const QNetworkReply::NetworkError &error)
{
    QMetaObject::invokeMethod(&m_requestsHandler, std::bind(&DataRequestsManager::offlineActionError, &m_requestsHandler,
                                                            packageId, action, error));
}

void DataManager::onAdditionalDataLoaded(const QJsonObject &additionalData)
{
    QMetaObject::invokeMethod(&m_requestsHandler, std::bind(&DataRequestsManager::processAdditionalData, &m_requestsHandler,
                                                            additionalData));
}

void DataManager::onEntitiesInfoLoaded(const QJsonArray &entitiesInfo)
{
    QMetaObject::invokeMethod(&m_requestsHandler, std::bind(&DataRequestsManager::processEntitiesInfo, &m_requestsHandler,
                                                            entitiesInfo));
}

void DataManager::onEntitiesLoaded(const QJsonArray &entities)
{
    QMetaObject::invokeMethod(&m_requestsHandler, std::bind(&DataRequestsManager::processEntities, &m_requestsHandler,
                                                            entities));
}

void DataManager::onUnusedLotIdsLoaded(const QJsonArray &idsArray)
{
    QMetaObject::invokeMethod(&m_requestsHandler, std::bind(&DataRequestsManager::processUnusedLotIdsLoaded, &m_requestsHandler,
                                                            idsArray));
}

void DataManager::setupHandlersConnections()
{
    auto updateProcessing = [this](bool childProcessing) {
        const auto singleProcessing = m_modelsHandler.processing() ^ m_requestsHandler.processing();
        const auto noneProcessing = !m_modelsHandler.processing() && !m_requestsHandler.processing();
        if ((singleProcessing && childProcessing) || noneProcessing) {
            emit processingChanged(this->processing());
        }
    };
    connect(&m_modelsHandler, &AbstractManager::processingChanged, this, updateProcessing);
    connect(&m_requestsHandler, &AbstractManager::processingChanged, this, updateProcessing);

    // data related
    connect(&m_modelsHandler, &DataModelsManager::limitEventsNeeded, this, &DataManager::fetchLimitEvents);
    connect(&m_modelsHandler, &DataModelsManager::eventsNeeded, this, &DataManager::eventsNeeded);
    connect(&m_modelsHandler, &DataModelsManager::packageData, this, &DataManager::packageData);
    connect(&m_modelsHandler, &DataModelsManager::offlineActions, &m_requestsHandler, &DataRequestsManager::processOfflineActions);

    connect(&m_requestsHandler, &DataRequestsManager::additionalDataProcessed, &m_modelsHandler, &DataModelsManager::processAdditionalData);
    connect(&m_requestsHandler, &DataRequestsManager::entitiesInfoProcessed, &m_modelsHandler, &DataModelsManager::processEntitiesInfo);
    connect(&m_requestsHandler, &DataRequestsManager::entitiesProcessed, &m_modelsHandler, &DataModelsManager::processEntities);
    connect(&m_requestsHandler, &DataRequestsManager::relationsProcessed, &m_modelsHandler, &DataModelsManager::processRelations);
    connect(&m_requestsHandler, &DataRequestsManager::unusedLotIdsProcessed, &m_modelsHandler, &DataModelsManager::processUnusedLotIds);

    connect(&m_requestsHandler, &DataRequestsManager::updateLocalAction, &m_modelsHandler, &DataModelsManager::updateLocalAction);
    connect(&m_requestsHandler, &DataRequestsManager::removeLocalAction, &m_modelsHandler, &DataModelsManager::removeLocalAction);

    connect(&m_requestsHandler, &DataRequestsManager::sendOfflineAction,
            this, [this](const QString &packageId, const Enums::SupplyChainAction &action,
            const QDateTime &timestamp, const QVariantMap &properties){
        emit addActionRequest(packageId, action, timestamp, properties);
    });
}

QString DataManager::generateHarvestId(const QDate &date, const QString &parcelCode)
{
    return date.toString(QStringLiteral("%1/d-M-yyyy")).arg(parcelCode);
}
