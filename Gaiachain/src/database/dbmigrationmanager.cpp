#include "dbmigrationmanager.h"

#include <QDebug>
#include <QStandardPaths>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QSqlQuery>
#include <QSqlError>

#include "dbhelpers.h"

using namespace db;

MigrationManager::MigrationManager(const QString &dbPath, QObject *parent)
    : QObject(parent), c_dbPath(dbPath)
{}

bool MigrationManager::checkAndCreate()
{
    auto exists = dbExist();
    if (!exists) {
        if (!QFileInfo(c_dbPath).absoluteDir().mkpath(".")) {
            qCCritical(databaseMigration) << "Cannot create a directory for database: " << c_dbPath;
        }
    }

    if (!openDb()) {
        qCCritical(databaseMigration) << "Cannot open the local database. File corrupted?";
        return false;
    }

    if (exists) {
        m_dbVersion = getVersionNumber();
    }
    return true;
}

bool MigrationManager::needsUpdate()
{
    return (m_dbVersion != LATEST_DB_VERSION);
}

bool MigrationManager::update()
{
    if (!needsUpdate()) {
        qCInfo(databaseMigration) << "Database up to date.";
        return true;
    }

    return updateDb();
}

bool MigrationManager::dbExist() const
{
    return QFile::exists(c_dbPath);
}

bool MigrationManager::openDb()
{
    db::Helpers::setupDatabaseConnection(m_db, c_dbPath, metaObject()->className());
    return m_db.open();
}

QVersionNumber MigrationManager::getVersionNumber() const
{
    static const QLatin1String VersionQuery = QLatin1String("SELECT `version` from `Migrations` ORDER BY `id` DESC LIMIT 1");

    auto query = QSqlQuery(m_db);
    query.prepare(VersionQuery);
    Helpers::execQuery(query, true);
    if (!query.first()) {
        qCCritical(databaseMigration) << "No version in migration table.";
        return {};
    }

    return QVersionNumber::fromString(query.value(0).toString());
}

bool MigrationManager::updateDb()
{
    if (m_dbVersion > LATEST_DB_VERSION) {
        // backward
        return applyMigrations(DB_MIGRATIONS.rbegin(), DB_MIGRATIONS.rend(), std::bind(Migration::RunBackward, std::placeholders::_1, m_db), false);
    }

    // forward
    return applyMigrations(DB_MIGRATIONS.begin(), DB_MIGRATIONS.end(), std::bind(Migration::RunForward, std::placeholders::_1, m_db), true);
}

template<typename It>
bool MigrationManager::applyMigrations(It begin, It end, std::function<bool(const Migration &)> const &handler, bool forward)
{
    auto start = begin;
    if (!m_dbVersion.isNull()) {
        start = findMigrationNumber(begin, end, m_dbVersion);
        if (start == end) {
            qCCritical(databaseMigration) << "Cannot update database - version missing.";
            return false;
        }
        start += (forward ? 1 : 0);
    }

    auto finish = findMigrationNumber(begin, end, LATEST_DB_VERSION);
    if (finish == end) {
        qCCritical(databaseMigration) << "Cannot update database - latest version missing.";
        return false;
    }
    finish += (forward ? 1 : 0);

    return std::all_of(start, finish, handler);
}

template<typename It>
It MigrationManager::findMigrationNumber(It begin, It end, const QVersionNumber &number)
{
    auto item = std::find_if(begin, end, [&number](const Migration &migration) { return (migration.number() == number); });
    if (item == end) {
        qCCritical(databaseMigration) << "Version not found in migrations! Version:" << number;
    }

    return item;
}
