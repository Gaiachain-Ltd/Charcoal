#include "unusedplotidsmodel.h"

#include "database/dbhelpers.h"

UnusedPlotIdsModel::UnusedPlotIdsModel(QObject *parent) : QSqlQueryModel(parent)
{
}

void UnusedPlotIdsModel::setDbConnection(const QString &connectionName)
{
    setQuery("SELECT name FROM Entities WHERE isFinished=0 AND typeId IN "
             "(SELECT id FROM EntityTypes WHERE name=\"Plot\")",
             db::Helpers::databaseConnection(connectionName));
}
