#include "dbhelpers.h"

#include <QSqlQuery>
#include <QSqlError>

#include "../common/logs.h"

Q_LOGGING_CATEGORY(databaseQuery, "database.query")

bool db::Helpers::hasError(const QSqlQuery &query)
{
    return query.lastError().isValid();
}

void db::Helpers::execQuery(QSqlQuery &query, bool critical)
{
    query.exec();
    if (hasError(query)) {
        qCLog(critical ? QtCriticalMsg : QtWarningMsg, databaseQuery())
                << "Cannot apply query: (" << query.lastQuery() << " ) error:" << query.lastError().text();
    }
}

bool db::Helpers::execQuery(const QSqlDatabase &db, const QLatin1String &queryStr, bool critical)
{
    auto query = QSqlQuery(db);
    query.prepare(queryStr);
    execQuery(query, critical);
    return !hasError(query);
}

bool db::Helpers::runQueries(const QSqlDatabase &db, const QList<QLatin1String> &queries, bool critical)
{
    return std::all_of(queries.constBegin(), queries.constEnd(),
                       std::bind(qOverload<const QSqlDatabase &, const QLatin1String &, bool>(&execQuery),
                                 db, std::placeholders::_1, critical));
}
