#ifndef SQLQUERYMODEL_H
#define SQLQUERYMODEL_H

#include <QSqlQueryModel>
#include <QSqlQuery>

#include <QDebug>
#include <QSqlError>

#include "isortfilterquery.h"
#include "../../database/dbhelpers.h"

class SqlQueryModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    SqlQueryModel(const QLatin1String &tableName, const QString &dbConnectionName, QObject *parent = nullptr);

    template <typename SFQ, typename = std::enable_if_t<std::is_base_of<ISortFilterQuery, SFQ>::value>>
    SqlQueryModel(const QLatin1String &tableName, const QString &dbConnectionName, const SFQ &query, QObject *parent = nullptr)
        : QSqlQueryModel(parent), c_dbConnectionName(dbConnectionName), m_tableName(tableName)
    {
        setSortFilterQuery(query);
    }

    template <typename SFQ>
    void setSortFilterQuery(const SFQ &queryData)
    {
        static const auto SelectQuery = QString{"SELECT * FROM `%1` %2;"};
        auto[ queryFilterStr, bindValues ] = queryData.resolve();

        m_queryStr = SelectQuery.arg(m_tableName).arg(queryFilterStr);
        m_bindValues = bindValues;

        select();
    }

    void select();

protected:
    const QString c_dbConnectionName;

    QString m_tableName;

    QString m_queryStr;
    QVariantList m_bindValues;
};

#endif // SQLQUERYMODEL_H
