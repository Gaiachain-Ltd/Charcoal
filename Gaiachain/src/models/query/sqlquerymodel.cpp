#include "sqlquerymodel.h"

SqlQueryModel::SqlQueryModel(const QLatin1String &tableName, const QString &dbConnectionName, QObject *parent)
    : QSqlQueryModel(parent), c_dbConnectionName(dbConnectionName), m_tableName(tableName)
{}

void SqlQueryModel::select()
{
    auto dbQuery = QSqlQuery(db::Helpers::databaseConnection(c_dbConnectionName));
    dbQuery.prepare(m_queryStr);
    std::for_each(m_bindValues.constBegin(), m_bindValues.constEnd(),
                  std::bind(&QSqlQuery::addBindValue, &dbQuery, std::placeholders::_1, QSql::In));

    dbQuery.exec();
    setQuery(dbQuery);
}
