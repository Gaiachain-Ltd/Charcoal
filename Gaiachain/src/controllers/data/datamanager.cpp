#include "datamanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QJsonObject>
#include <QJsonArray>

#include "../../common/dataglobals.h"
#include "../../helpers/packagedataproperties.h"

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

    m_viewModelsHandler.setupQmlContext(engine);
}

void DataManager::setupDatabase(const QString &dbPath)
{
    QMetaObject::invokeMethod(&m_modelsHandler, std::bind(&DataModelsManager::setupDatabaseModels, &m_modelsHandler, dbPath));
    m_viewModelsHandler.setupDatabaseModels(dbPath);
}

void DataManager::updateUserData(const UserData &userData)
{
    m_userData = userData;
    m_viewModelsHandler.updateCooperativeId(userData.cooperativeId);
}

bool DataManager::processing() const
{
    return (m_modelsHandler.processing() || m_requestsHandler.processing());
}

void DataManager::getPackageData(const QString &packageId) const
{
    const auto data = m_viewModelsHandler.getPackageData(packageId);
    emit packageData(data);
}

QString DataManager::generateHarvestId(const QDate &date, const QString &parcelCode)
{
    return date.toString(QStringLiteral("%1/d-M-yyyy")).arg(parcelCode);
}

void DataManager::addAction(const QString &packageId, const Enums::SupplyChainAction &action,
                            const QDateTime &timestamp, const QVariantMap &properties)
{
    const auto isOfflineAction = DataGlobals::availableOfflineActions().contains(action) && !m_userData.isAnonymous();
    if (isOfflineAction) {
        QMetaObject::invokeMethod(&m_modelsHandler, std::bind(&DataModelsManager::addLocalAction, &m_modelsHandler,
                                                              packageId, action, timestamp, m_userData.cooperativeId, properties));
    } else {
        emit addActionRequest(packageId, action, timestamp, properties);
    }
}

void DataManager::addAction(const QString &packageId, const Enums::SupplyChainAction &action, const QByteArray &codeData,
                            const QDateTime &timestamp, const QVariantMap &properties)
{
    emit addActionRequest(packageId, codeData, action, timestamp, properties);
}

void DataManager::addAction(const Enums::SupplyChainAction &action, const QByteArray &codeData,
                            const QDateTime &timestamp, const QVariantMap &properties)
{
    emit addActionRequest(codeData, action, timestamp, properties);
}

void DataManager::sendOfflineActions()
{
    const auto offlineActions = m_viewModelsHandler.getOfflineActions();
    QMetaObject::invokeMethod(const_cast<DataRequestsManager *>(&m_requestsHandler),    // const cast required to use invokeMethod. Method called is const.
                              std::bind(&DataRequestsManager::processOfflineActions, &m_requestsHandler, offlineActions));
}

void DataManager::fetchEventData(const QString &packageId, const Enums::PackageType &type)
{
    QMetaObject::invokeMethod(&m_modelsHandler, std::bind(&DataModelsManager::processPackageData, &m_modelsHandler,
                                                          packageId, type));
}

void DataManager::fetchRangeEvents(const QDateTime &from, const QDateTime &to)
{
    emit eventsInfoNeeded(from, to);
}

void DataManager::fetchLimitRangeEvents(int limit, int offset, const QDateTime &from, const QDateTime &to)
{
    emit eventsInfoNeeded(limit, offset, from, to);
}

void DataManager::fetchLimitKeywordEvents(int limit, int offset, const QString &keyword)
{
    emit eventsInfoNeeded(limit, offset, keyword);
}

void DataManager::fetchLastActionPackageEvents(const Enums::SupplyChainAction &lastAction)
{
    emit lastActionEventsInfoNeeded(lastAction);
}

void DataManager::onActionAdded(const QString &packageId, const QByteArray &, const Enums::SupplyChainAction &action)
{
    const auto isOfflineAction = DataGlobals::availableOfflineActions().contains(action) && !m_userData.isAnonymous();
    if (isOfflineAction) {
        QMetaObject::invokeMethod(&m_requestsHandler, std::bind(&DataRequestsManager::offlineActionAdded, &m_requestsHandler,
                                                                packageId, action));
    }
}

void DataManager::onActionAddError(const QString &packageId, const QByteArray &, const Enums::SupplyChainAction &action,
                                   const QNetworkReply::NetworkError &error)
{
    const auto isOfflineAction = DataGlobals::availableOfflineActions().contains(action) && !m_userData.isAnonymous();
    if (isOfflineAction) {
        QMetaObject::invokeMethod(&m_requestsHandler, std::bind(&DataRequestsManager::offlineActionError, &m_requestsHandler,
                                                                packageId, action, error));
    }
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
    connect(&m_modelsHandler, &DataModelsManager::modelUpdated, &m_viewModelsHandler, &DataViewModelsManager::onModelUpdated);

    connect(&m_viewModelsHandler, &DataViewModelsManager::packagesEventsNeeded, &m_modelsHandler, &DataModelsManager::processPackagesInfo);
    connect(&m_viewModelsHandler, &DataViewModelsManager::limitKeywordEventsNeeded, this, &DataManager::fetchLimitKeywordEvents);
    connect(&m_viewModelsHandler, &DataViewModelsManager::limitRangeEventsNeeded, this, &DataManager::fetchLimitRangeEvents);

    connect(&m_modelsHandler, &DataModelsManager::eventsNeeded, this, &DataManager::eventsNeeded);
    connect(&m_modelsHandler, &DataModelsManager::eventInserted, this, &DataManager::eventInserted);
    connect(&m_modelsHandler, &DataModelsManager::relationInserted, this, &DataManager::relationInserted);

    connect(&m_requestsHandler, &DataRequestsManager::additionalDataProcessed, &m_modelsHandler, &DataModelsManager::processAdditionalData);
    connect(&m_requestsHandler, &DataRequestsManager::entitiesInfoProcessed, &m_modelsHandler, &DataModelsManager::processEntitiesInfo);
    connect(&m_requestsHandler, &DataRequestsManager::entitiesProcessed, &m_modelsHandler, &DataModelsManager::processEntities);
    connect(&m_requestsHandler, &DataRequestsManager::relationsProcessed, &m_modelsHandler, &DataModelsManager::processRelations);
    connect(&m_requestsHandler, &DataRequestsManager::unusedLotIdsProcessed, &m_modelsHandler, &DataModelsManager::processUnusedLotIds);

    connect(&m_requestsHandler, &DataRequestsManager::updateLocalAction, &m_modelsHandler, &DataModelsManager::updateLocalAction);
    connect(&m_requestsHandler, &DataRequestsManager::removeLocalAction, &m_modelsHandler, &DataModelsManager::removeLocalAction);

    connect(&m_modelsHandler, &DataModelsManager::localActionAdded, this, &DataManager::localActionAdded);
    connect(&m_modelsHandler, &DataModelsManager::localActionDuplicated, this, &DataManager::localActionDuplicated);

    connect(&m_modelsHandler, &DataModelsManager::localActionAdded,
            this, qOverload<const QString &, const Enums::SupplyChainAction &,
            const QDateTime &, const QVariantMap &>(&DataManager::addActionRequest));
    connect(&m_modelsHandler, &DataModelsManager::localActionDuplicated,
            this, qOverload<const QString &, const Enums::SupplyChainAction &,
            const QDateTime &, const QVariantMap &>(&DataManager::addActionRequest));

    connect(&m_requestsHandler, &DataRequestsManager::sendOfflineAction,
            this, [this](const QString &packageId, const Enums::SupplyChainAction &action,
            const QDateTime &timestamp, const QVariantMap &properties){
        emit addActionRequest(packageId, action, timestamp, properties);
    });
}