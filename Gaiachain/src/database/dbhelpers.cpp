#include "dbhelpers.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QThread>

#include "common/logs.h"

Q_LOGGING_CATEGORY(databaseQuery, "database.query")

namespace {
    QString connectionThreadName(const QString &connectionName)
    {
        auto currentThreadIdx = QString("0x%1").arg((quintptr)(QThread::currentThread()),
                                                    QT_POINTER_SIZE * 2, 16, QChar('0'));

        return QString("%1_%2").arg(connectionName).arg(currentThreadIdx);
    }
}

bool db::Helpers::setupDatabaseConnection(const QString &dbPath, const QString &connectionName)
{
    auto db = QSqlDatabase::addDatabase("QSQLITE", connectionThreadName(connectionName));
    db.setDatabaseName(dbPath);
    if (!db.open()) {
        Q_ASSERT(db.isOpen());
        qCritical(databaseQuery) << RED("Cannot open database connection. Cannot proceed.");
    }

    return db.isOpen();
}

bool db::Helpers::hasDatabaseConnection(const QString &connectionName)
{
    return QSqlDatabase::contains(connectionThreadName(connectionName));
}

QSqlDatabase db::Helpers::databaseConnection(const QString &connectionName)
{
    Q_ASSERT(hasDatabaseConnection(connectionName));
    return QSqlDatabase::database(connectionThreadName(connectionName));
}

bool db::Helpers::hasError(const QSqlQuery &query)
{
    return query.lastError().isValid();
}

void db::Helpers::execQuery(QSqlQuery &query, bool critical)
{
    query.exec();
    if (hasError(query)) {
        qCLog(critical ? QtCriticalMsg : QtWarningMsg, databaseQuery())
            << RED("Cannot apply query:") << "(" << query.lastQuery() << ")"
            << RED("error:") << query.lastError().text();
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
