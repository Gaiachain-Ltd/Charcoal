#include "oventypesmodel.h"

#include "database/dbhelpers.h"

OvenTypesModel::OvenTypesModel(QObject *parent) : QSqlQueryModel(parent)
{
}

void OvenTypesModel::setDbConnection(const QString &connectionName)
{
    setQuery("SELECT name FROM OvenTypes",
             db::Helpers::databaseConnection(connectionName));
}
