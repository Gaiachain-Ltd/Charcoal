#include "charcoaldatamanager.h"

#include "database/dbhelpers.h"
#include "common/logs.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QDebug>

CharcoalDataManager::CharcoalDataManager(const QSharedPointer<RestSessionManager> &sessionManager,
                                         const QSharedPointer<UserManager> &userManager,
                                         QObject *parent)
    : AbstractDataManager(parent),
      m_sessionManager(sessionManager),
      m_userManager(userManager),
      m_actionController(new ActionController(this)),
      m_treeSpeciesModel(new TreeSpeciesModel(this)),
      m_villagesModel(new VillagesModel(this)),
      m_parcelsModel(new ParcelsModel(this)),
      m_destinationsModel(new DestinationsModel(this)),
      m_ovenTypesModel(new OvenTypesModel(this)),
      m_unusedPlotIdsModel(new UnusedPlotIdsModel(this)),
      m_unusedHarvestIdsModel(new UnusedHarvestIdsModel(this)),
      m_unusedTransportIdsModel(new UnusedTransportIdsModel(this)),
      m_unusedPlotIdsForReplantationModel(new UnusedPlotIdsForReplantationModel(this)),
      m_ovensModel(new OvensModel(this)),
      m_trackingModel(new TrackingModel(this)),
      m_trackingFilterProxyModel(new TrackingFilterProxyModel(this)),
      m_minimumDateModel(new MinimumDateModel(this)),
      m_localEventsModel(new LocalEventsModel(this)),
      m_replantationsSender(new ReplantationsSender(this)),
      m_eventsSender(new EventsSender(this))
{
    m_trackingFilterProxyModel->setSourceModel(m_trackingModel);

    connect(m_actionController, &ActionController::refreshLocalEvents,
            m_localEventsModel, &LocalEventsModel::refresh);

    connect(m_actionController, &ActionController::refreshLocalEvents,
            m_replantationsSender, &ReplantationsSender::refresh);

    connect(m_actionController, &ActionController::refreshLocalEvents,
            m_eventsSender, &EventsSender::refresh);

    connect(m_replantationsSender, &ReplantationsSender::refreshed,
            m_localEventsModel, &LocalEventsModel::refresh);

    connect(m_eventsSender, &EventsSender::refreshed,
            m_localEventsModel, &LocalEventsModel::refresh);

    connect(m_trackingModel, &TrackingModel::finalizePackages,
            m_eventsSender, &EventsSender::onFinalizePackages);

    connect(m_actionController, &ActionController::finalizePackages,
            m_eventsSender, &EventsSender::onFinalizePackages);
}

void CharcoalDataManager::setupDatabase(const QString &dbPath)
{
    qDebug() << "Setting DB connections in CDM" << dbPath;

    m_dbPath = dbPath;
    db::Helpers::setupDatabaseConnection(dbPath, m_dbConnectionName);

    m_actionController->setDbConnection(m_dbConnectionName);

    setupModel(m_treeSpeciesModel);
    setupModel(m_villagesModel);
    setupModel(m_parcelsModel);
    setupModel(m_destinationsModel);
    setupModel(m_ovenTypesModel);
    setupModel(m_unusedPlotIdsModel);
    setupModel(m_unusedHarvestIdsModel);
    setupModel(m_unusedTransportIdsModel);
    setupModel(m_unusedPlotIdsForReplantationModel);
    setupModel(m_ovensModel);
    setupModel(m_trackingModel);
    setupModel(m_minimumDateModel);
    setupModel(m_localEventsModel);

    setupModel(m_replantationsSender);
    setupModel(m_eventsSender);

    if (checkModels() == false) {
        qWarning() << RED("Data models are initialized improperly!");
    }
}

void CharcoalDataManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    AbstractDataManager::setupQmlContext(engine);
    engine.rootContext()->setContextProperty(QStringLiteral("localEventsModel"), m_localEventsModel);
}

void CharcoalDataManager::setPicturesManager(PicturesManager *manager)
{
    if (m_actionController) {
        m_actionController->setPicturesManager(manager);
    }

    if (m_eventsSender) {
        m_eventsSender->setPicturesManager(manager);
    }

    if (m_trackingModel) {
        m_trackingModel->setPicturesManager(manager);
    }
}

void CharcoalDataManager::sendOfflineActions()
{
    const auto models = findChildren<QueryModel*>();

    for (const auto model : models) {
        if (model == nullptr) {
            continue;
        }

        if (model->hasQueuedRequests()) {
            model->sendQueuedRequests();
        }
    }
}

TreeSpeciesModel *CharcoalDataManager::treeSpeciesModel() const
{
    return m_treeSpeciesModel;
}

VillagesModel *CharcoalDataManager::villagesModel() const
{
    return m_villagesModel;
}

ParcelsModel *CharcoalDataManager::parcelsModel() const
{
    return m_parcelsModel;
}

DestinationsModel *CharcoalDataManager::destinationsModel() const
{
    return m_destinationsModel;
}

OvenTypesModel *CharcoalDataManager::ovenTypesModel() const
{
    return m_ovenTypesModel;
}

ActionController *CharcoalDataManager::actionController() const
{
    return m_actionController;
}

UnusedPlotIdsModel *CharcoalDataManager::unusedPlotIdsModel() const
{
    return m_unusedPlotIdsModel;
}

UnusedHarvestIdsModel *CharcoalDataManager::unusedHarvestIdsModel() const
{
    return m_unusedHarvestIdsModel;
}

UnusedTransportIdsModel *CharcoalDataManager::unusedTransportIdsModel() const
{
    return m_unusedTransportIdsModel;
}

UnusedPlotIdsForReplantationModel *CharcoalDataManager::unusedPlotIdsForReplantationModel() const
{
    return m_unusedPlotIdsForReplantationModel;
}

OvensModel *CharcoalDataManager::ovensModel() const
{
    return m_ovensModel;
}

TrackingModel *CharcoalDataManager::trackingModel() const
{
    return m_trackingModel;
}

TrackingFilterProxyModel *CharcoalDataManager::trackingFilterProxyModel() const
{
    return m_trackingFilterProxyModel;
}

MinimumDateModel *CharcoalDataManager::minimumDateModel() const
{
    return m_minimumDateModel;
}

LocalEventsModel *CharcoalDataManager::localEventsModel() const
{
    return m_localEventsModel;
}

bool CharcoalDataManager::checkModels() const
{
    const auto models = findChildren<QueryModel*>();

    for (const auto model : models) {
        if (model == nullptr) {
            return false;
        }

        if (model->isValid() == false) {
            qWarning() << "Uninitialized model" << model->metaObject()->className();
            return false;
        }
    }

    return true;
}

void CharcoalDataManager::setupModel(QueryModel *model) const
{
    model->setSessionManager(m_sessionManager);
    model->setUserManager(m_userManager);
    model->setDbConnection(m_dbConnectionName);

    connect(model, &QueryModel::error,
            this, &CharcoalDataManager::error);
}
