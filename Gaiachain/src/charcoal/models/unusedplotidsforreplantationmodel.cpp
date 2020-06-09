#include "unusedplotidsforreplantationmodel.h"

#include "database/dbhelpers.h"

UnusedPlotIdsForReplantationModel::UnusedPlotIdsForReplantationModel(QObject *parent)
    : QSqlQueryModel(parent)
{
}

void UnusedPlotIdsForReplantationModel::setDbConnection(const QString &connectionName)
{
    setQuery("SELECT name FROM Entities WHERE isReplanted=0 AND typeId IN "
             "(SELECT id FROM EntityTypes WHERE name=\"Plot\")",
             db::Helpers::databaseConnection(connectionName));
}
