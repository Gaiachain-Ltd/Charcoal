#include "abstractdatamanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QJsonObject>
#include <QJsonArray>
#include <QGeoCoordinate>

#include "../../common/dataglobals.h"
#include "../../helpers/requestshelper.h"
#include "../../helpers/packagedataproperties.h"

#include <QLoggingCategory>
Q_LOGGING_CATEGORY(dataManager, "data.manager")

AbstractDataManager::AbstractDataManager(QObject *parent)
    : AbstractManager(parent)
{
    m_processingThread.start();
    m_modelsHandler.moveToThread(&m_processingThread);
    m_modelsHandler.updateThread();
    m_localHandler.moveToThread(&m_processingThread);
    m_requestsHandler.moveToThread(&m_processingThread);

    setupHandlersConnections();
}

AbstractDataManager::~AbstractDataManager()
{
    m_processingThread.quit();
    m_processingThread.wait();
}

void AbstractDataManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("dataManager"), this);

    m_viewHandler.setupQmlContext(engine);
}

void AbstractDataManager::setupDatabase(const QString &dbPath)
{
    QMetaObject::invokeMethod(&m_modelsHandler, std::bind(&DataModelsManager::setupDatabaseModels, &m_modelsHandler, dbPath));
    m_viewHandler.setupDatabaseModels(dbPath);
}

void AbstractDataManager::updateUserData(const UserData &userData)
{
    m_userData = userData;
#ifdef COCOA
    m_viewHandler.updateCooperativeId(userData.cooperativeId);
#elif CHARCOAL
    qDebug() << "TODO!";
#endif
}

bool AbstractDataManager::processing() const
{
    return (m_modelsHandler.processing() || m_requestsHandler.processing());
}

void AbstractDataManager::preparePackageData(const QString &packageId)
{
    const auto data = m_viewHandler.preparePackageData(packageId);
    emit packageData(data);
}

QString AbstractDataManager::generateHarvestId(const QDate &date, const QString &parcelCode)
{
    return date.toString(QStringLiteral("%1/d-M-yyyy")).arg(parcelCode);
}

void AbstractDataManager::addAction(const QString &packageId,
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

void AbstractDataManager::addAction(const QString &packageId, const Enums::SupplyChainAction &action, const QByteArray &codeData,
                            const QGeoCoordinate &coordinate, const QDateTime &timestamp, const QVariantMap &properties)
{
    emit addActionRequest(packageId, codeData, action, coordinate, timestamp, properties);
}

void AbstractDataManager::addAction(const Enums::SupplyChainAction &action, const QByteArray &codeData,
                            const QGeoCoordinate &coordinate, const QDateTime &timestamp, const QVariantMap &properties)
{
    emit addActionRequest(codeData, action, coordinate, timestamp, properties);
}

void AbstractDataManager::sendOfflineActions()
{
    const auto offlineActions = m_viewHandler.getOfflineActions();
    QMetaObject::invokeMethod(const_cast<DataRequestsManager *>(&m_requestsHandler),    // const cast required to use invokeMethod. Method called is const.
                              std::bind(&DataRequestsManager::processOfflineActions, &m_requestsHandler, offlineActions));
}

void AbstractDataManager::removeOfflineAction(const QString &packageId, const Enums::SupplyChainAction &action)
{
    QMetaObject::invokeMethod(&m_modelsHandler, std::bind(&DataModelsManager::removeLocalAction, &m_modelsHandler,
                                                          packageId, action));
    QMetaObject::invokeMethod(&m_localHandler, std::bind(&DataLocalManager::removeLocalAction, &m_localHandler,
                                                         packageId, action));
}

void AbstractDataManager::fetchEventData(const QString &packageId, const Enums::PackageType &type)
{
    QMetaObject::invokeMethod(&m_modelsHandler, std::bind(&DataModelsManager::processPackageData, &m_modelsHandler,
                                                          packageId, type));
}

void AbstractDataManager::fetchRangeEvents(const QDateTime &from, const QDateTime &to)
{
    emit eventsInfoNeeded(from, to);
}

void AbstractDataManager::fetchLimitRangeEvents(int limit, int offset, const QDateTime &from, const QDateTime &to)
{
    emit eventsInfoNeeded(limit, offset, from, to);
}

void AbstractDataManager::fetchLimitKeywordEvents(int limit, int offset, const QString &keyword,
                                          const QSet<Enums::PackageType> &filteredPackages, int cooperativeId)
{
    emit eventsInfoNeeded(limit, offset, keyword, filteredPackages, cooperativeId);
}

void AbstractDataManager::fetchLastActionPackageEvents(const Enums::SupplyChainAction &lastAction)
{
    emit lastActionEventsInfoNeeded(lastAction);
}

void AbstractDataManager::onActionAdded(const QString &packageId, const QByteArray &, const Enums::SupplyChainAction &action)
{
    const auto isOfflineAction = DataGlobals::availableOfflineActions().contains(action);
    if (isOfflineAction) {
        QMetaObject::invokeMethod(&m_requestsHandler, std::bind(&DataRequestsManager::offlineActionAdded, &m_requestsHandler,
                                                                packageId, action));
    }
}

void AbstractDataManager::onActionAddError(const QString &packageId, const QByteArray &, const Enums::SupplyChainAction &action,
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

void AbstractDataManager::onAdditionalDataLoaded(const QJsonObject &additionalData)
{
    QMetaObject::invokeMethod(&m_requestsHandler, std::bind(&DataRequestsManager::processAdditionalData, &m_requestsHandler,
                                                            additionalData));
}

void AbstractDataManager::onEntitiesInfoLoaded(const QJsonArray &entitiesInfo)
{
    QMetaObject::invokeMethod(&m_requestsHandler, std::bind(&DataRequestsManager::processEntitiesInfo, &m_requestsHandler,
                                                            entitiesInfo));
}

void AbstractDataManager::onEntitiesLoaded(const QJsonArray &entities)
{
    QMetaObject::invokeMethod(&m_requestsHandler, std::bind(&DataRequestsManager::processEntities, &m_requestsHandler,
                                                            entities));
}

void AbstractDataManager::setupHandlersConnections()
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

    connect(&m_modelsHandler, &DataModelsManager::eventsNeeded, this, &AbstractDataManager::eventsNeeded);
    connect(&m_modelsHandler, &DataModelsManager::eventInserted, this, &AbstractDataManager::eventInserted);
    connect(&m_modelsHandler, &DataModelsManager::relationInserted, this, &AbstractDataManager::relationInserted);

    connect(&m_viewHandler, &DataViewManager::packagesEventsNeeded, &m_modelsHandler, &DataModelsManager::processPackagesInfo);
    connect(&m_viewHandler, &DataViewManager::limitKeywordEventsNeeded, this, &AbstractDataManager::fetchLimitKeywordEvents);
    connect(&m_viewHandler, &DataViewManager::limitRangeEventsNeeded, this, &AbstractDataManager::fetchLimitRangeEvents);

    connect(&m_requestsHandler, &DataRequestsManager::updateOfflineAction, &m_modelsHandler, &DataModelsManager::updateLocalAction);
    connect(&m_requestsHandler, &DataRequestsManager::updateOfflineAction, &m_localHandler, &DataLocalManager::removeLocalAction);

    connect(&m_modelsHandler, &DataModelsManager::localActionAdded, this, &AbstractDataManager::localActionAdded);
    connect(&m_modelsHandler, &DataModelsManager::localActionDuplicated, this, &AbstractDataManager::localActionDuplicated);
    connect(&m_localHandler, &DataLocalManager::localActionDataError, this, &AbstractDataManager::localActionDataError);

    connect(&m_modelsHandler, &DataModelsManager::localActionAdded,
            this, qOverload<const QString &, const Enums::SupplyChainAction &,
            const QGeoCoordinate &, const QDateTime &, const QVariantMap &>(&AbstractDataManager::addActionRequest));
    // adding action even if duplicated, not to rely on local data (server will respond with duplicated error)
    connect(&m_modelsHandler, &DataModelsManager::localActionDuplicated,
            this, qOverload<const QString &, const Enums::SupplyChainAction &,
            const QGeoCoordinate &, const QDateTime &, const QVariantMap &>(&AbstractDataManager::addActionRequest));

    connect(&m_requestsHandler, &DataRequestsManager::sendOfflineAction,
            &m_localHandler, &DataLocalManager::sendLocalAction);
    connect(&m_localHandler, &DataLocalManager::localActionDataError,
            &m_requestsHandler, &DataRequestsManager::offlineActionError);
    connect(&m_localHandler, &DataLocalManager::localActionAddRequest,
            this, qOverload<const QString &, const Enums::SupplyChainAction &,
            const QGeoCoordinate &, const QDateTime &, const QVariantMap &>(&AbstractDataManager::addActionRequest));
}
