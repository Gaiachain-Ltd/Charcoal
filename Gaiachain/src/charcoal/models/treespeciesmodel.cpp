#include "treespeciesmodel.h"

#include "database/dbhelpers.h"

TreeSpeciesModel::TreeSpeciesModel(QObject *parent) : QSqlQueryModel(parent)
{
}

void TreeSpeciesModel::setDbConnection(const QString &connectionName)
{
    setQuery("SELECT name FROM TreeSpecies",
             db::Helpers::databaseConnection(connectionName));
}
