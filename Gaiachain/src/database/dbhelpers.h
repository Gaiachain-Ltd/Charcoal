#ifndef DBHELPERS_H
#define DBHELPERS_H

#include "dbmigrations.h"

#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(databaseQuery)

class QSqlQuery;

namespace db {
class Helpers
{
public:
    static bool hasError(const QSqlQuery &query);

    static void execQuery(QSqlQuery &query, bool critical = false);
    static bool execQuery(const QSqlDatabase &db, const QLatin1String &queryStr, bool critical = false);

    static bool runQueries(const QSqlDatabase &db, const QList<QLatin1String> &queries, bool critical = false);
};

}

#endif // DBHELPERS_H
