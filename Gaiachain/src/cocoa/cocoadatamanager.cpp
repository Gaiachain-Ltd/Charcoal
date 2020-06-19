#include "cocoadatamanager.h"

#include "common/dataglobals.h"
#include "helpers/requestshelper.h"
#include "helpers/packagedataproperties.h"

#include <QMetaObject>
#include <QJsonObject>
#include <QJsonArray>

CocoaDataManager::CocoaDataManager(QObject *parent)
    : AbstractDataManager(parent)
{
    // Warning: thread is already running!
    m_modelsHandler.moveToThread(&m_processingThread);
    m_modelsHandler.updateThread();
    m_localHandler.moveToThread(&m_processingThread);
    m_requestsHandler.moveToThread(&m_processingThread);

    setupHandlersConnections();
}

QString CocoaDataManager::generateHarvestId(const QDate &date, const QString &parcelCode)
{
    return date.toString(QStringLiteral("%1/d-M-yyyy")).arg(parcelCode);
}

void CocoaDataManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    AbstractDataManager::setupQmlContext(engine);
    m_viewHandler.setupQmlContext(engine);
}

void CocoaDataManager::onUnusedLotIdsLoaded(const QJsonArray &idsArray)
{
    QMetaObject::invokeMethod(
        &m_requestsHandler,
        std::bind(&DataRequestsManager::processUnusedLotIdsLoaded,
                  &m_requestsHandler,
                  idsArray));
}

void CocoaDataManager::setupDatabase(const QString &dbPath)
{
    QMetaObject::invokeMethod(&m_modelsHandler, std::bind(&DataModelsManager::setupDatabaseModels, &m_modelsHandler, dbPath));
    m_viewHandler.setupDatabaseModels(dbPath);
}

void CocoaDataManager::updateUserData(const UserData &userData)
{
    AbstractDataManager::updateUserData(userData);
    m_viewHandler.updateCooperativeId(userData.cooperativeId);
}
bool CocoaDataManager::processing() const
{
    return (m_modelsHandler.processing() || m_requestsHandler.processing());
}

void CocoaDataManager::preparePackageData(const QString &packageId)
{
    const auto data = m_viewHandler.preparePackageData(packageId);
    emit packageData(data);
}

void CocoaDataManager::addAction(const QString &packageId,
                                    const Enums::SupplyChainAction &action,
                                    const QGeoCoordinate &coordinate,
                                    const QDateTime &timestamp,
                                    const QVariantMap &properties)
{
    const auto isOfflineAction = DataGlobals::availableOfflineActions().contains(action);
    if (isOfflineAction && !m_userData.isAnonymous()) {
#ifdef COCOA
        QMetaObject::invokeMethod(&m_modelsHandler,
                                  std::bind(&DataModelsManager::addLocalAction,
                                            &m_modelsHandler, packageId, action,
                                            coordinate, timestamp,
                                            m_userData.cooperativeId, properties));
#elif CHARCOAL
        qDebug() << "TODO!";
#endif

        QMetaObject::invokeMethod(&m_localHandler,
                                  std::bind(&DataLocalManager::addLocalAction,
                                            &m_localHandler, packageId, action,
                                            coordinate, timestamp, properties));
    } else {
        emit addActionRequest(packageId, action, coordinate, timestamp, properties);
    }
}

void CocoaDataManager::addAction(const QString &packageId, const Enums::SupplyChainAction &action, const QByteArray &codeData,
                                    const QGeoCoordinate &coordinate, const QDateTime &timestamp, const QVariantMap &properties)
{
    emit addActionRequest(packageId, codeData, action, coordinate, timestamp, properties);
}

void CocoaDataManager::addAction(const Enums::SupplyChainAction &action, const QByteArray &codeData,
                                    const QGeoCoordinate &coordinate, const QDateTime &timestamp, const QVariantMap &properties)
{
    emit addActionRequest(codeData, action, coordinate, timestamp, properties);
}

void CocoaDataManager::sendOfflineActions()
{
    const auto offlineActions = m_viewHandler.getOfflineActions();
    QMetaObject::invokeMethod(const_cast<DataRequestsManager *>(&m_requestsHandler),    // const cast required to use invokeMethod. Method called is const.
                              std::bind(&DataRequestsManager::processOfflineActions, &m_requestsHandler, offlineActions));
}

void CocoaDataManager::removeOfflineAction(const QString &packageId, const Enums::SupplyChainAction &action)
{
    QMetaObject::invokeMethod(&m_modelsHandler, std::bind(&DataModelsManager::removeLocalAction, &m_modelsHandler,
                                                          packageId, action));
    QMetaObject::invokeMethod(&m_localHandler, std::bind(&DataLocalManager::removeLocalAction, &m_localHandler,
                                                         packageId, action));
}

void CocoaDataManager::fetchEventData(const QString &packageId, const Enums::PackageType &type)
{
    QMetaObject::invokeMethod(&m_modelsHandler, std::bind(&DataModelsManager::processPackageData, &m_modelsHandler,
                                                          packageId, type));
}

void CocoaDataManager::fetchRangeEvents(const QDateTime &from, const QDateTime &to)
{
    emit eventsInfoNeeded(from, to);
}

void CocoaDataManager::fetchLimitRangeEvents(int limit, int offset, const QDateTime &from, const QDateTime &to)
{
    emit eventsInfoNeeded(limit, offset, from, to);
}

void CocoaDataManager::fetchLimitKeywordEvents(int limit, int offset, const QString &keyword,
                                                  const QSet<Enums::PackageType> &filteredPackages, int cooperativeId)
{
    emit eventsInfoNeeded(limit, offset, keyword, filteredPackages, cooperativeId);
}

void CocoaDataManager::fetchLastActionPackageEvents(const Enums::SupplyChainAction &lastAction)
{
    emit lastActionEventsInfoNeeded(lastAction);
}

void CocoaDataManager::onActionAdded(const QString &packageId, const QByteArray &, const Enums::SupplyChainAction &action)
{
    const auto isOfflineAction = DataGlobals::availableOfflineActions().contains(action);
    if (isOfflineAction) {
        QMetaObject::invokeMethod(&m_requestsHandler, std::bind(&DataRequestsManager::offlineActionAdded, &m_requestsHandler,
                                                                packageId, action));
    }
}

void CocoaDataManager::onActionAddError(const QString &packageId, const QByteArray &, const Enums::SupplyChainAction &action,
                                           const QNetworkReply::NetworkError &error)
{
    const auto isOfflineAction = DataGlobals::availableOfflineActions().contains(action);
    if (isOfflineAction) {
        QMetaObject::invokeMethod(&m_requestsHandler, std::bind(&DataRequestsManager::offlineActionError, &m_requestsHandler,
                                                                packageId, action));
        if (!RequestsHelper::isOfflineError(error)) {
            removeOfflineAction(packageId, action);
        }
    }
}

void CocoaDataManager::onAdditionalDataLoaded(const QJsonObject &additionalData)
{
    QMetaObject::invokeMethod(&m_requestsHandler, std::bind(&DataRequestsManager::processAdditionalData, &m_requestsHandler,
                                                            additionalData));
}

void CocoaDataManager::onEntitiesInfoLoaded(const QJsonArray &entitiesInfo)
{
    QMetaObject::invokeMethod(&m_requestsHandler, std::bind(&DataRequestsManager::processEntitiesInfo, &m_requestsHandler,
                                                            entitiesInfo));
}

void CocoaDataManager::onEntitiesLoaded(const QJsonArray &entities)
{
    QMetaObject::invokeMethod(&m_requestsHandler, std::bind(&DataRequestsManager::processEntities, &m_requestsHandler,
                                                            entities));
}

void CocoaDataManager::setupHandlersConnections()
{
    auto updateProcessing = [this](bool childProcessing) {
        const auto processingCount = m_requestsHandler.processing() + m_modelsHandler.processing() +
            m_localHandler.processing() + m_viewHandler.processing();
        const auto singleProcessing = (processingCount == 1);
        const auto noneProcessing = (processingCount == 0);

        if ((singleProcessing && childProcessing) || noneProcessing) {
            emit processingChanged(this->processing());
        }
    };
    connect(&m_requestsHandler, &AbstractManager::processingChanged, this, updateProcessing);
    connect(&m_modelsHandler, &AbstractManager::processingChanged, this, updateProcessing);
    connect(&m_localHandler, &AbstractManager::processingChanged, this, updateProcessing);
    connect(&m_viewHandler, &AbstractManager::processingChanged, this, updateProcessing);

    // data related
    connect(&m_modelsHandler, &DataModelsManager::modelUpdated, &m_viewHandler, &DataViewManager::onModelUpdated);

    connect(&m_requestsHandler, &DataRequestsManager::additionalDataProcessed, &m_modelsHandler, &DataModelsManager::processAdditionalData);
    connect(&m_requestsHandler, &DataRequestsManager::entitiesInfoProcessed, &m_modelsHandler, &DataModelsManager::processEntitiesInfo);
    connect(&m_requestsHandler, &DataRequestsManager::entitiesProcessed, &m_modelsHandler, &DataModelsManager::processEntities);
    connect(&m_requestsHandler, &DataRequestsManager::relationsProcessed, &m_modelsHandler, &DataModelsManager::processRelations);
    connect(&m_requestsHandler, &DataRequestsManager::unusedLotIdsProcessed, &m_modelsHandler, &DataModelsManager::processUnusedLotIds);

    connect(&m_modelsHandler, &DataModelsManager::eventsNeeded, this, &CocoaDataManager::eventsNeeded);
    connect(&m_modelsHandler, &DataModelsManager::eventInserted, this, &CocoaDataManager::eventInserted);
    connect(&m_modelsHandler, &DataModelsManager::relationInserted, this, &CocoaDataManager::relationInserted);

    connect(&m_viewHandler, &DataViewManager::packagesEventsNeeded, &m_modelsHandler, &DataModelsManager::processPackagesInfo);
    connect(&m_viewHandler, &DataViewManager::limitKeywordEventsNeeded, this, &CocoaDataManager::fetchLimitKeywordEvents);
    connect(&m_viewHandler, &DataViewManager::limitRangeEventsNeeded, this, &CocoaDataManager::fetchLimitRangeEvents);

    connect(&m_requestsHandler, &DataRequestsManager::updateOfflineAction, &m_modelsHandler, &DataModelsManager::updateLocalAction);
    connect(&m_requestsHandler, &DataRequestsManager::updateOfflineAction, &m_localHandler, &DataLocalManager::removeLocalAction);

    connect(&m_modelsHandler, &DataModelsManager::localActionAdded, this, &CocoaDataManager::localActionAdded);
    connect(&m_modelsHandler, &DataModelsManager::localActionDuplicated, this, &CocoaDataManager::localActionDuplicated);
    connect(&m_localHandler, &DataLocalManager::localActionDataError, this, &CocoaDataManager::localActionDataError);

    connect(&m_modelsHandler, &DataModelsManager::localActionAdded,
            this, qOverload<const QString &, const Enums::SupplyChainAction &,
                            const QGeoCoordinate &, const QDateTime &, const QVariantMap &>(&CocoaDataManager::addActionRequest));
    // adding action even if duplicated, not to rely on local data (server will respond with duplicated error)
    connect(&m_modelsHandler, &DataModelsManager::localActionDuplicated,
            this, qOverload<const QString &, const Enums::SupplyChainAction &,
                            const QGeoCoordinate &, const QDateTime &, const QVariantMap &>(&CocoaDataManager::addActionRequest));

    connect(&m_requestsHandler, &DataRequestsManager::sendOfflineAction,
            &m_localHandler, &DataLocalManager::sendLocalAction);
    connect(&m_localHandler, &DataLocalManager::localActionDataError,
            &m_requestsHandler, &DataRequestsManager::offlineActionError);
    connect(&m_localHandler, &DataLocalManager::localActionAddRequest,
            this, qOverload<const QString &, const Enums::SupplyChainAction &,
                            const QGeoCoordinate &, const QDateTime &, const QVariantMap &>(&CocoaDataManager::addActionRequest));
}
