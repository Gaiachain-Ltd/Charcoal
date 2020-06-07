#include "entitiesmodel.h"

#include "database/dbhelpers.h"

EntitiesModel::EntitiesModel(QObject *parent) : QSqlQueryModel(parent)
{
}

void EntitiesModel::setDbConnection(const QString &connectionName)
{
    setQuery("SELECT name FROM Entities",
             db::Helpers::databaseConnection(connectionName));

    // TODO: we need to also return Events for each Entity!
}
