#include "sqlquerymodel.h"

SqlQueryModel::SqlQueryModel(const QLatin1String &tableName, QSqlDatabase db, QObject *parent)
    : QSqlQueryModel(parent), m_db(db), m_tableName(tableName)
{}

void SqlQueryModel::select()
{
    m_dbQuery.exec();
    setQuery(m_dbQuery);
}
