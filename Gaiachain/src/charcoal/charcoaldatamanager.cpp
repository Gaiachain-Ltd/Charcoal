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
      m_ovenTypesModel(new OvenTypesModel(this))
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
}

QString CharcoalDataManager::generatePlotId(const QString &userId,
                                            const QString &parcelCode,
                                            const QDate &date)
{
    return userId + sep + parcelCode + sep + date.toString(dateFormat);
}

QString CharcoalDataManager::generateHarvestId(const QString &plotId,
                                               const QString &userId)
{
    return plotId + sep + userId;
}

QString CharcoalDataManager::generateTransportId(const QString &harvestId,
                                                 const QString &licensePlate,
                                                 const int transportNumber,
                                                 const QDate &date)
{
    return harvestId + sep + licensePlate
        + sep + "T" + QString::number(transportNumber)
        + sep + date.toString(dateFormat);
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
