#include "databasemanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>

#include <QLoggingCategory>
Q_LOGGING_CATEGORY(databaseManager, "database.manager")

const QLatin1String DatabaseManager::sc_dbName = QLatin1String("local.db");

DatabaseManager::DatabaseManager(QObject *parent)
    : AbstractManager(parent), c_dbPath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/" + sc_dbName),
      m_migrationManager(c_dbPath)
{
    qCDebug(databaseManager) << "DB path:" << c_dbPath;

    connect(&m_migrationProgress, &QFutureWatcher<bool>::finished,
            this, [this]() {
        processFinished();
        if (!m_migrationProgress.result()) {
            emit databaseUpdateError();
        }

        emit databaseReady(dbPath());
    });
}

void DatabaseManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("dbManager"), this);
}

void DatabaseManager::setupDatabase()
{
    ProcessCounter p(this);

    Q_ASSERT_X(!m_setupDone, __PRETTY_FUNCTION__, "Trying to setup database twice");
    if (m_setupDone || !m_migrationManager.checkAndCreate()) {
        return;
    }

    if (m_migrationManager.needsUpdate()) {
        processStarted();

        m_migrationRunner = QtConcurrent::run(std::bind(&db::MigrationManager::update, &m_migrationManager));
        m_migrationProgress.setFuture(m_migrationRunner);
    } else {
        emit databaseReady(dbPath());
    }

    m_setupDone = true;
}

QString DatabaseManager::dbPath() const
{
    return c_dbPath;
}
