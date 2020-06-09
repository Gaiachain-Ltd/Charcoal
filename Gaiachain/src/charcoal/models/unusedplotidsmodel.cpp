#include "unusedplotidsmodel.h"

#include "database/dbhelpers.h"

UnusedPlotIdsModel::UnusedPlotIdsModel(QObject *parent) : QSqlQueryModel(parent)
{
}

void UnusedPlotIdsModel::setDbConnection(const QString &connectionName)
{
    // TODO: there is a difference between unused PlotId that can be
    // used in Logging Ending and Carbonisation Beginning!
    setQuery("SELECT name FROM Entities WHERE isFinished=0 AND typeId IN "
             "(SELECT id FROM EntityTypes WHERE name=\"Plot\")",
             db::Helpers::databaseConnection(connectionName));
}
