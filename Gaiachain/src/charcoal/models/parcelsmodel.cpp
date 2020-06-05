#include "parcelsmodel.h"

#include "database/dbhelpers.h"

ParcelsModel::ParcelsModel(QObject *parent) : QSqlQueryModel(parent)
{
}

void ParcelsModel::setDatabasePath(const QString &connectionName)
{
    setQuery("SELECT name FROM Parcels",
             db::Helpers::databaseConnection(connectionName));
}
