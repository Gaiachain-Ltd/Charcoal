#ifndef ABSTRACTDATAMODELSMANAGER_H
#define ABSTRACTDATAMODELSMANAGER_H

#include "../abstractmanager.h"

#include <QSqlDatabase>

class AbstractDataModelsManager : public AbstractManager
{
    Q_OBJECT
public:
    enum class ModelType {
        Producers,
        Parcels,
        Companies,
        Destinations,
        Events,
        Relations,
        UnusedIds
    };

    AbstractDataModelsManager(QObject *parent = nullptr);

    void setupDatabaseModels(const QString &dbPath);

protected:
    static const QMap<ModelType, QLatin1String> sc_databaseTableName;

    QSqlDatabase m_db;

    void setupDatabase(const QString &dbPath);
    virtual void setupModels() = 0;
    virtual void setupUpdateConnections() = 0;
};

#endif // ABSTRACTDATAMODELSMANAGER_H
