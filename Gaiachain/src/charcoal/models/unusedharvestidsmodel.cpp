#include "unusedharvestidsmodel.h"

#include "database/dbhelpers.h"

UnusedHarvestIdsModel::UnusedHarvestIdsModel(QObject *parent) : QSqlQueryModel(parent)
{
}

void UnusedHarvestIdsModel::setDbConnection(const QString &connectionName)
{
    // TODO: there is a difference between unused HarvestId that can be
    // used in Carbonisation Ending and Loading&Transport!
    setQuery("SELECT name FROM Entities WHERE isUsed=0 AND typeId IN "
             "(SELECT id FROM EntityTypes WHERE name=\"Harvest\")",
             db::Helpers::databaseConnection(connectionName));
}
