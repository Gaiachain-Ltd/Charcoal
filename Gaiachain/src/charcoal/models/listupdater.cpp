#include "listupdater.h"

#include "database/dbhelpers.h"
#include "common/logs.h"
#include "common/tags.h"

#include <QVariant>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <QSqlQuery>
#include <QSqlError>

ListUpdater::ListUpdater(const QString &tableName, const QString &connectionName)
    : m_tableName(tableName), m_connectionName(connectionName)
{
    if (isValid() == false) {
        qWarning() << RED("Invalid table name or connection name!")
                   << m_tableName << "#" << m_connectionName;
    }
}

bool ListUpdater::updateTable(const QJsonDocument &webData,
                              const QString &fieldName) const
{
    return updateTable(webData, QStringList(fieldName));
}

bool ListUpdater::updateTable(const QJsonDocument &webData,
                              const QStringList &fieldNames) const
{
    QStringList names;
    if (fieldNames.contains(Tags::id)) {
        names = fieldNames;
    } else {
        names.append(Tags::id);
        names.append(fieldNames);
    }

    const auto webItems = webList(names, webData);
    const auto dbItems = dbList(names);
    return (removeObsoleteItems(webItems, dbItems)
            && insertMissingItems(webItems, dbItems));
}

RecordsList ListUpdater::webList(const QStringList &fieldNames,
                                 const QJsonDocument &json) const
{
    RecordsList result;
    const QJsonObject mainObject(json.object());
    const QJsonArray mainArray(mainObject.value(Tags::results).toArray());

    for (const QJsonValue &item : mainArray) {
        Record record;
        const QJsonObject object(item.toObject());
        for (const QString &name : fieldNames) {
            record.insert(name, object.value(name).toVariant());
        }
        result.append(record);
    }

    return result;
}

RecordsList ListUpdater::dbList(const QStringList &fieldNames) const
{
    RecordsList result;

    const QString queryString("SELECT %1 FROM %2");
    QSqlQuery query(queryString.arg(fieldNames.join(sep), m_tableName),
                    db::Helpers::databaseConnection(m_connectionName));

    query.exec();
    while(query.next()) {
        Record record;
        for (const QString &name : fieldNames) {
            record.insert(name, query.value(name).toString());
        }
        result.append(record);
    }

    return result;
}

bool ListUpdater::insertMissingItems(const RecordsList &webItems,
                                     const RecordsList &dbItems) const
{
    RecordsList toInsert;
    for (const auto &webItem : qAsConst(webItems)) {
        if (dbItems.contains(webItem) == false) {
            toInsert.append(webItem);
        }
    }

    const QString insert("INSERT INTO %1 (%2) VALUES (%3)");
    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));

    for (const auto &item : qAsConst(toInsert)) {
        query.prepare(insert.arg(m_tableName,
                                 item.keys().join(sep),
                                 wrapAndJoin(item.values())));

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

bool ListUpdater::removeObsoleteItems(const RecordsList &webItems,
                                      const RecordsList &dbItems) const
{
    RecordsList toRemove;

    for (const auto &localItem : qAsConst(dbItems)) {
        if (webItems.contains(localItem) == false) {
            toRemove.append(localItem);
        }
    }

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));

    const QString remove("DELETE FROM %1 WHERE %2=%3");

    for (const auto &item : qAsConst(toRemove)) {
        query.prepare(remove.arg(m_tableName, Tags::id, item.value(Tags::id).toString()));
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

bool ListUpdater::isValid() const
{
    return (m_tableName.isEmpty() == false && m_connectionName.isEmpty() == false);
}

QString ListUpdater::wrapAndJoin(const QList<QVariant> &items) const
{
    QString result;

    for (const QVariant &item : items) {
        if (result.isEmpty() == false) {
            result.append(sep);
        }

        const auto type = item.type();
        switch (type) {
        case QVariant::Type::Bool:
            result.append(QString::number(item.toBool()));
            break;
        case QVariant::Type::Int:
            result.append(QString::number(item.toInt()));
            break;
        case QVariant::Type::String:
        default:
            result.append(wrap + item.toString() + wrap);
            break;

        }
    }

    return result;
}
