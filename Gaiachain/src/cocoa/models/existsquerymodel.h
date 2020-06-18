#ifndef EXISTSQUERYMODEL_H
#define EXISTSQUERYMODEL_H

#include <QSqlQuery>

#include "common/globals.h"

class ExistsQueryModel : public QObject
{
    Q_OBJECT
public:
    explicit ExistsQueryModel(QObject *parent = nullptr);
    ExistsQueryModel(const QString &dbConnectionName, QObject *parent = nullptr);

    bool prepareQuery(const QString &tableName, const QStringList &conditionFields);

    bool exists(const QString &tableName, const QVariantMap &conditions);
    bool exists(const QVariantList &conditionValues);

private:
    const QString c_dbConnectionName;

    QString m_queryStr;
    QStringList m_conditionFields;
};

#endif // EXISTSQUERYMODEL_H
