#include "listmodelhelper.h"

#include "database/dbhelpers.h"
#include "common/logs.h"

#include <QVariant>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <QSqlQuery>
#include <QSqlError>

Updates::Updates(const QString &tableName, const QString &connectionName)
    : m_tableName(tableName), m_connectionName(connectionName)
{
    if (isValid() == false) {
        qWarning() << RED("Invalid table name or connection name!")
                   << m_tableName << "#" << m_connectionName;
    }
}

bool Updates::updateTable(const QString &fieldName,
                          const QJsonDocument &webData,
                          QSqlQuery query)
{
    const QStringList webItems = webList(fieldName, webData);
    const QStringList dbItems = dbList(fieldName, query);
    return (insertMissingItems(webItems, dbItems)
            && removeObsoleteItems(webItems, dbItems));
}

QStringList Updates::webList(const QString &fieldName, const QJsonDocument &json)
{
    QStringList result;
    const QJsonObject mainObject(json.object());
    const QJsonArray mainArray(mainObject.value("results").toArray());

    for (const QJsonValue &item : mainArray) {
        const QJsonObject object(item.toObject());
        result.append(object.value(fieldName).toString());
    }

    return result;
}

QStringList Updates::dbList(const QString &fieldName, QSqlQuery query)
{
    QStringList result;

    query.seek(-1);
    while(query.next()) {
        result.append(query.value(fieldName).toString());
    }

    return result;
}

bool Updates::insertMissingItems(const QStringList &webItems,
                                 const QStringList &dbItems) const
{
    QStringList toInsert;
    for (const QString &webItem : qAsConst(webItems)) {
        if (dbItems.contains(webItem) == false) {
            toInsert.append(webItem);
        }
    }

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));

    const QString insert("INSERT INTO TreeSpecies (name) VALUES (\"%1\")");
    for (const QString &item : qAsConst(toInsert)) {
        query.prepare(insert.arg(item));
        if (query.exec() == false) {
            qWarning() << RED("Inserting TreeSpecies has failed!")
                       << query.lastError().text()
                       << "for query:" << query.lastQuery()
                       << "DB:" << m_connectionName;
            return false;
        }
    }

    return true;
}

bool Updates::removeObsoleteItems(const QStringList &webItems,
                                  const QStringList &dbItems) const
{
    QStringList toRemove;

    for (const QString &localItem : qAsConst(dbItems)) {
        if (webItems.contains(localItem) == false) {
            toRemove.append(localItem);
        }
    }

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));

    const QString remove("DELETE FROM TreeSpecies WHERE name=\"%1\"");
    for (const QString &item : qAsConst(toRemove)) {
        query.prepare(remove.arg(item));
        if (query.exec() == false) {
            qWarning() << RED("Removing TreeSpecies has failed!")
                       << query.lastError().text()
                       << "for query:" << query.lastQuery()
                       << "DB:" << m_connectionName;
            return false;
        }
    }

    return true;
}

bool Updates::isValid()
{
    return (m_tableName.isEmpty() == false && m_connectionName.isEmpty() == false);
}
