#include "charcoaldatamanager.h"

#include "database/dbhelpers.h"

#include <QDate>

#include <QDebug>

CharcoalDataManager::CharcoalDataManager(QObject *parent)
    : AbstractDataManager(parent),
      m_treeSpeciesModel(new TreeSpeciesModel(this))
{
}

void CharcoalDataManager::setupDatabase(const QString &dbPath)
{
    qDebug() << "Setting DB connections in CDM" << dbPath;

    m_dbPath = dbPath;

    db::Helpers::setupDatabaseConnection(dbPath, m_dbConnectionName);
    m_treeSpeciesModel->setDatabasePath(m_dbConnectionName);
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
