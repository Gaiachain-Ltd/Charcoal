#pragma once

#include <QStringList>

class QJsonDocument;
class QSqlQuery;

class TableUpdater
{
public:
    TableUpdater(const QString &tableName, const QString &connectionName);

    bool updateTable(const QString &fieldName, const QJsonDocument &webData);

private:
    QStringList webList(const QString &fieldName, const QJsonDocument &json);
    QStringList dbList(const QString &fieldName);
    bool insertMissingItems(const QString &fieldName,
                            const QStringList &webItems,
                            const QStringList &dbItems) const;
    bool removeObsoleteItems(const QString &fieldName,
                             const QStringList &webItems,
                             const QStringList &dbItems) const;

    bool isValid();

    QString m_tableName;
    QString m_connectionName;
};

