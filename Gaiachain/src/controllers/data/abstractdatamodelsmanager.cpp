#include "abstractdatamodelsmanager.h"

#include <QCoreApplication>

#include "../../database/dbhelpers.h"

const QMap<AbstractDataModelsManager::ModelType, QLatin1String> AbstractDataModelsManager::sc_databaseTableName = {
    { ModelType::Producers, QLatin1String("Producers") },
    { ModelType::Parcels, QLatin1String("Parcels") },
    { ModelType::Companies, QLatin1String("Companies") },
    { ModelType::Destinations, QLatin1String("Destinations") },

    { ModelType::Events, QLatin1String("Events") },
    { ModelType::Relations, QLatin1String("Relations") },
    { ModelType::UnusedIds, QLatin1String("UnusedIds") },
};

AbstractDataModelsManager::AbstractDataModelsManager(QObject *parent)
    : AbstractManager(parent)
{}

void AbstractDataModelsManager::setupDatabaseModels(const QString &dbPath)
{
    setupDatabase(dbPath);
    setupModels();
    setupUpdateConnections();
}

void AbstractDataModelsManager::setupDatabase(const QString &dbPath)
{
    db::Helpers::setupDatabaseConnection(m_db, dbPath, metaObject()->className());
}

namespace
{
    void registerTypes()
    {
        qRegisterMetaType<AbstractDataModelsManager::ModelType>("AbstractDataModelsManager::ModelType");
    }
}
Q_COREAPP_STARTUP_FUNCTION(registerTypes)
