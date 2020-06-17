#include "querymodel.h"

#include "database/dbhelpers.h"
#include "common/logs.h"

#include <QDebug>

QueryModel::QueryModel(QObject *parent) : QSqlQueryModel(parent)
{
}

void QueryModel::setDbQuery(const QString &query)
{
    m_query = query;
}

void QueryModel::setDbConnection(const QString &connectionName)
{
    m_connectionName = connectionName;
    refresh();
}

bool QueryModel::isValid() const
{
    return m_connectionName.isEmpty() == false;
}

void QueryModel::refresh()
{
    if (m_query.isEmpty()) {
        qWarning() << RED("Cannot establish DB connection when query is empty!")
                   << m_connectionName;
        return;
    }

    if (m_connectionName.isEmpty()) {
        qWarning() << RED("Cannot establish DB connection when connection name is empty!")
                   << m_query;
        return;
    }

    setQuery(m_query, db::Helpers::databaseConnection(m_connectionName));
}
