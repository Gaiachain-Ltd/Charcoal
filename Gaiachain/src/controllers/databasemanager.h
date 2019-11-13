#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "abstractmanager.h"

#include <QFutureWatcher>

#include "../database/dbmigrationmanager.h"

class DatabaseManager : public AbstractManager
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = nullptr);

    virtual void setupQmlContext(QQmlApplicationEngine &engine) override;

    void setupDatabase();
    QSqlDatabase database() const;

signals:
    void databaseReady(QSqlDatabase db);

    void databaseUpdateStarted() const;
    void databaseUpdateFinished(bool success) const;

private:
    bool m_setupDone = false;

    QSqlDatabase m_db;

    db::MigrationManager m_migrationManager;
    QFuture<bool> m_migrationRunner;
    QFutureWatcher<bool> m_migrationProgress;

    void onMigrationFinished(bool result);
};

#endif // DATABASEMANAGER_H
