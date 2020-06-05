#include "treespeciesmodel.h"

#include "database/dbhelpers.h"

#include <QSqlDatabase>
#include <QSqlQuery>

TreeSpeciesModel::TreeSpeciesModel(QObject *parent) : QSqlQueryModel(parent)
{
}

void TreeSpeciesModel::setDatabasePath(const QString &connectionName)
{
    setQuery("SELECT name FROM TreeSpecies",
             db::Helpers::databaseConnection(connectionName));
}
