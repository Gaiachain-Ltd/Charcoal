#include "dbmigrations.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>

#include "dbhelpers.h"

Q_LOGGING_CATEGORY(databaseMigration, "database.migration")

using namespace db;

Migration::Migration(const QVersionNumber &number, const std::function<bool (QSqlDatabase &)> &forward, const std::function<bool (QSqlDatabase &)> &backward)
    : m_number(number), m_forward(forward), m_backward(backward)
{}

bool Migration::RunForward(const Migration &m, QSqlDatabase &db)
{
    m.runCommon(db);
    auto result = m.m_forward(db);

    static const QLatin1String addMigrationEntryQuery = QLatin1String("INSERT INTO Migrations (`timestamp`, `version`) VALUES (:timestamp, :number)");
    auto query = QSqlQuery(db);
    query.prepare(addMigrationEntryQuery);
    query.bindValue(":timestamp", QDateTime::currentDateTime().toSecsSinceEpoch());
    query.bindValue(":number", m.m_number.toString());

    Helpers::execQuery(query);
    result &= !Helpers::hasError(query);

    return result;
}

bool Migration::RunBackward(const Migration &m, QSqlDatabase &db)
{
    m.runCommon(db);
    auto result = m.m_backward(db);

    static const QLatin1String removeMigrationEntryQuery = QLatin1String("DELETE FROM `Migrations` WHERE `version` IS :number");
    auto query = QSqlQuery(db);
    query.prepare(removeMigrationEntryQuery);
    query.bindValue(":number", m.m_number.toString());

    Helpers::execQuery(query);
    result &= !Helpers::hasError(query);

    return result;
}

QVersionNumber Migration::number() const
{
    return m_number;
}

void Migration::runCommon(QSqlDatabase &db) const
{
    Q_ASSERT_X(db.isOpen(), __PRETTY_FUNCTION__,
               "Database object should always be opened for migration!");
}
