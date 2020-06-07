#include "destinationsmodel.h"

#include "database/dbhelpers.h"

DestinationsModel::DestinationsModel(QObject *parent) : QSqlQueryModel(parent)
{

}

void DestinationsModel::setDbConnection(const QString &connectionName)
{
    setQuery("SELECT name FROM Destinations",
             db::Helpers::databaseConnection(connectionName));
}
