#ifndef MIGRATIONS_H
#define MIGRATIONS_H

#include <QVersionNumber>
#include <functional>
#include <QVector>

#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(databaseMigration)

class QSqlDatabase;

namespace db {

class Migration;

//!!! Always remember to update current version for any db changes!
const QVersionNumber LATEST_DB_VERSION = { 0, 9, 1 };

extern const QVector<Migration> DB_MIGRATIONS;

class Migration {
public:
    Migration(const QVersionNumber &m_number,
              const std::function<bool(QSqlDatabase &)> &m_forward,
              const std::function<bool(QSqlDatabase &)> &m_backward);

    static bool RunForward(const Migration &m, QSqlDatabase &db);
    static bool RunBackward(const Migration &m, QSqlDatabase &db);

    QVersionNumber number() const;

private:
    QVersionNumber m_number;
    std::function<bool(QSqlDatabase &)> m_forward;
    std::function<bool(QSqlDatabase &)> m_backward;

    void runCommon(QSqlDatabase &db) const;
};

}
#endif // MIGRATIONS_H
