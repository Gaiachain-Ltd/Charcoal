#ifndef EXISTSQUERYMODEL_H
#define EXISTSQUERYMODEL_H

#include <QSqlQuery>

#include "../common/globals.h"

class ExistsQueryModel : public QObject
{
    Q_OBJECT
public:
    explicit ExistsQueryModel(QObject *parent = nullptr);
    ExistsQueryModel(QSqlDatabase db, QObject *parent = nullptr);

    void setDatabase(QSqlDatabase db);

    bool prepareQuery(const QString &tableName, const QStringList &conditionFields);

    bool exists(const QString &tableName, const QVariantMap &conditions);
    bool exists(const QVariantList &conditionValues);

private:
    QSqlDatabase m_db;

    QStringList m_conditionFields;
    QSqlQuery m_query;
};

#endif // EXISTSQUERYMODEL_H
