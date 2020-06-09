#include "unusedtransportidsmodel.h"

#include "database/dbhelpers.h"

UnusedTransportIdsModel::UnusedTransportIdsModel(QObject *parent) : QSqlQueryModel(parent)
{
}

void UnusedTransportIdsModel::setDbConnection(const QString &connectionName)
{
    setQuery("SELECT name FROM Entities WHERE isFinished=0 AND typeId IN "
             "(SELECT id FROM EntityTypes WHERE name=\"Transport\")",
             db::Helpers::databaseConnection(connectionName));
}
