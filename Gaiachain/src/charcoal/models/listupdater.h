#pragma once

#include <QHash>
#include <QString>
#include <QStringList>

using Record = QHash<QString, QString>;
using RecordsList = QVector<Record>;

class QJsonDocument;
class QSqlQuery;

class ListUpdater
{
public:
    ListUpdater(const QString &tableName, const QString &connectionName);

    bool updateTable(const QJsonDocument &webData,
                     const QString &fieldName) const;

    bool updateTable(const QJsonDocument &webData,
                     const QStringList &fieldNames) const;

private:
    RecordsList webList(const QStringList &fieldNames,
                        const QJsonDocument &json) const;

    RecordsList dbList(const QStringList &fieldNames) const;

    bool insertMissingItems(const RecordsList &webItems,
                            const RecordsList &dbItems) const;

    bool removeObsoleteItems(const RecordsList &webItems,
                             const RecordsList &dbItems) const;

    bool isValid() const;

    QString wrapAndJoin(const QStringList &items) const;

    const QLatin1String wrap = QLatin1String("\"");
    const QLatin1String sep = QLatin1String(", ");

    QString m_tableName;
    QString m_connectionName;
};

