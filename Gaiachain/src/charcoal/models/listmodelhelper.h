#pragma once

#include <QStringList>

class QJsonDocument;
class QSqlQuery;

class TableUpdater
{
public:
    TableUpdater(const QString &tableName, const QString &connectionName);

    bool updateTable(const QString &fieldName, const QJsonDocument &webData,
                     QSqlQuery query);

private:
    QStringList webList(const QString &fieldName, const QJsonDocument &json);
    QStringList dbList(const QString &fieldName, QSqlQuery query);
    bool insertMissingItems(const QStringList &webItems,
                            const QStringList &dbItems) const;
    bool removeObsoleteItems(const QStringList &webItems,
                             const QStringList &dbItems) const;

    bool isValid();

    QString m_tableName;
    QString m_connectionName;
};

