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

TableUpdater::TableUpdater(const QString &tableName, const QString &connectionName)
    : m_tableName(tableName), m_connectionName(connectionName)
{
    if (isValid() == false) {
        qWarning() << RED("Invalid table name or connection name!")
                   << m_tableName << "#" << m_connectionName;
    }
}

bool TableUpdater::updateTable(const QString &fieldName,
                               const QJsonDocument &webData)
{
    const QStringList webItems = webList(fieldName, webData);
    const QStringList dbItems = dbList(fieldName);
    return (insertMissingItems(fieldName, webItems, dbItems)
            && removeObsoleteItems(fieldName, webItems, dbItems));
}

QStringList TableUpdater::webList(const QString &fieldName, const QJsonDocument &json)
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

QStringList TableUpdater::dbList(const QString &fieldName)
{
    QStringList result;

    const QString queryString("SELECT %1 FROM %2");
    QSqlQuery query(queryString.arg(fieldName, m_tableName),
                    db::Helpers::databaseConnection(m_connectionName));

    query.exec();
    while(query.next()) {
        result.append(query.value(fieldName).toString());
    }

    return result;
}

bool TableUpdater::insertMissingItems(const QString &fieldName,
                                      const QStringList &webItems,
                                      const QStringList &dbItems) const
{
    QStringList toInsert;
    for (const QString &webItem : qAsConst(webItems)) {
        if (dbItems.contains(webItem) == false) {
            toInsert.append(webItem);
        }
    }

    qDebug() << "Web" << webItems << "\nDB" << dbItems << "toInsert" << toInsert;

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));

    const QString insert("INSERT INTO %1 (%2) VALUES (\"%3\")");
    for (const QString &item : qAsConst(toInsert)) {
        query.prepare(insert.arg(m_tableName, fieldName, item));
        if (query.exec() == false) {
            qWarning() << RED("Inserting") << m_tableName << ("has failed!")
                       << query.lastError().text()
                       << "for query:" << query.lastQuery()
                       << "DB:" << m_connectionName;
            return false;
        }
    }

    return true;
}

bool TableUpdater::removeObsoleteItems(const QString &fieldName,
                                       const QStringList &webItems,
                                       const QStringList &dbItems) const
{
    QStringList toRemove;

    for (const QString &localItem : qAsConst(dbItems)) {
        if (webItems.contains(localItem) == false) {
            toRemove.append(localItem);
        }
    }

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));

    const QString remove("DELETE FROM %1 WHERE %2=\"%3\"");
    for (const QString &item : qAsConst(toRemove)) {
        query.prepare(remove.arg(m_tableName, fieldName, item));
        if (query.exec() == false) {
            qWarning() << RED("Removing") << m_tableName << ("has failed!")
                       << query.lastError().text()
                       << "for query:" << query.lastQuery()
                       << "DB:" << m_connectionName;
            return false;
        }
    }

    return true;
}

bool TableUpdater::isValid()
{
    return (m_tableName.isEmpty() == false && m_connectionName.isEmpty() == false);
}
