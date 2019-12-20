#ifndef SQLQUERYMODEL_H
#define SQLQUERYMODEL_H

#include <QSqlQueryModel>
#include <QSqlQuery>

#include <QDebug>
#include <QSqlError>

#include "isortfilterquery.h"

class SqlQueryModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    SqlQueryModel(const QLatin1String &tableName, QSqlDatabase db, QObject *parent = nullptr);

    template <typename SFQ, typename = std::enable_if_t<std::is_base_of<ISortFilterQuery, SFQ>::value>>
    SqlQueryModel(const QLatin1String &tableName, QSqlDatabase db, const SFQ &query, QObject *parent = nullptr)
        : QSqlQueryModel(parent), m_db(db), m_tableName(tableName)
    {
        setSortFilterQuery(query);
    }

    template <typename SFQ>
    void setSortFilterQuery(const SFQ &queryData)
    {
        static const auto SelectQuery = QString{"SELECT * FROM `%1` %2;"};
        auto [ queryFilterStr, bindValues ] = queryData.resolve();
        auto queryStr = SelectQuery.arg(m_tableName).arg(queryFilterStr);

        m_dbQuery = QSqlQuery(m_db);
        m_dbQuery.prepare(queryStr);
        for (const auto &bindValue : bindValues) {
            m_dbQuery.addBindValue(bindValue);
        }

        select();
    }

    void select();

protected:
    QSqlDatabase m_db;
    QSqlQuery m_dbQuery;

    QString m_tableName;

};

#endif // SQLQUERYMODEL_H
