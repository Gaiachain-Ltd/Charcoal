#include "charcoaldbhelpers.h"

#include "common/logs.h"
#include "database/dbhelpers.h"

#include <QString>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

int CharcoalDbHelpers::getWebPackageId(const QString &connectionName, const QString &entityId)
{
    return getSimpleInteger(connectionName, "Entities", "id", entityId, "webId");
}

QString CharcoalDbHelpers::getOvenLetter(const QString &connectionName, const QString &ovenId)
{
    QString result;
    const QString queryString(QString("SELECT name FROM Ovens "
                                      "WHERE id=%1").arg(ovenId));

    QSqlQuery query(queryString, db::Helpers::databaseConnection(connectionName));
    if (query.exec() && query.next()) {
        result = query.value("name").toString();
    } else {
        qWarning() << RED("Unable to fetch package ID (Web)")
                   << query.lastError() << "For query:" << query.lastQuery();
    }

    return result;
}

int CharcoalDbHelpers::getVillageId(const QString &connectionName, const QString &name)
{
    return getSimpleInteger(connectionName, "Villages", "name", name, "id");
}

int CharcoalDbHelpers::getDestinationId(const QString &connectionName, const QString &name)
{
    return getSimpleInteger(connectionName, "Destinations", "name", name, "id");
}

int CharcoalDbHelpers::getParcelId(const QString &connectionName, const QString &parcel)
{
    return getSimpleInteger(connectionName, "Parcels", "code", parcel, "id");
}

int CharcoalDbHelpers::getSimpleInteger(const QString &connectionName,
                                        const QString &table,
                                        const QString &matchColumn,
                                        const QString &matchValue,
                                        const QString &returnColumn)
{
    int result = -1;
    const QString queryString(QString("SELECT %1 FROM %2 WHERE %3=:value").arg(
        returnColumn, table, matchColumn));

    QSqlQuery query(QString(), db::Helpers::databaseConnection(connectionName));
    query.prepare(queryString);
    query.bindValue(":value", matchValue);

    if (query.exec() && query.next()) {
        result = query.value(returnColumn).toInt();
    } else {
        qWarning() << RED("Unable to fetch")
                   << connectionName << table << matchColumn << matchValue
                   << returnColumn
                   << "SQL error:" << query.lastError()
                   << "For query:" << query.lastQuery();
    }

    return result;
}
