#include "villagesmodel.h"

#include "database/dbhelpers.h"

VillagesModel::VillagesModel(QObject *parent) : QSqlQueryModel(parent)
{
}

void VillagesModel::setDbConnection(const QString &connectionName)
{
    setQuery("SELECT name FROM Villages",
             db::Helpers::databaseConnection(connectionName));
}
