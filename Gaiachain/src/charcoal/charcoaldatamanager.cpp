#include "charcoaldatamanager.h"

#include "database/dbhelpers.h"

#include <QDate>

#include <QDebug>

CharcoalDataManager::CharcoalDataManager(QObject *parent)
    : AbstractDataManager(parent),
      m_treeSpeciesModel(new TreeSpeciesModel(this)),
      m_villagesModel(new VillagesModel(this)),
      m_parcelsModel(new ParcelsModel(this)),
      m_destinationsModel(new DestinationsModel(this)),
      m_ovenTypesModel(new OvenTypesModel(this)),
      m_actionController(new ActionController(this)),
      m_unusedPlotIdsModel(new UnusedPlotIdsModel(this)),
      m_unusedHarvestIdsModel(new UnusedHarvestIdsModel(this)),
      m_unusedTransportIdsModel(new UnusedTransportIdsModel(this)),
      m_unusedPlotIdsForReplantationModel(new UnusedPlotIdsForReplantationModel(this)),
      m_ovensModel(new OvensModel(this))
{
}

void CharcoalDataManager::setupDatabase(const QString &dbPath)
{
    qDebug() << "Setting DB connections in CDM" << dbPath;

    m_dbPath = dbPath;

    db::Helpers::setupDatabaseConnection(dbPath, m_dbConnectionName);
    m_treeSpeciesModel->setDbConnection(m_dbConnectionName);
    m_villagesModel->setDbConnection(m_dbConnectionName);
    m_parcelsModel->setDbConnection(m_dbConnectionName);
    m_destinationsModel->setDbConnection(m_dbConnectionName);
    m_ovenTypesModel->setDbConnection(m_dbConnectionName);
    m_actionController->setDbConnection(m_dbConnectionName);
    m_unusedPlotIdsModel->setDbConnection(m_dbConnectionName);
    m_unusedHarvestIdsModel->setDbConnection(m_dbConnectionName);
    m_unusedTransportIdsModel->setDbConnection(m_dbConnectionName);
    m_unusedPlotIdsForReplantationModel->setDbConnection(m_dbConnectionName);
    m_ovensModel->setDbConnection(m_dbConnectionName);
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
