#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "abstractmanager.h"

#include <QFutureWatcher>

#include "database/dbmigrationmanager.h"

class DatabaseManager : public AbstractManager
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = nullptr);

    void setupQmlContext(QQmlApplicationEngine &engine) override;

    void setupDatabase();
    QString dbPath() const;

signals:
    void databaseReady(const QString &dbPath);
    void databaseUpdateError() const;

private:
    static const QLatin1String sc_dbName;
    const QString c_dbPath;

    bool m_setupDone = false;

    db::MigrationManager m_migrationManager;
    QFuture<bool> m_migrationRunner;
    QFutureWatcher<bool> m_migrationProgress;

    void onMigrationFinished(bool result);
};

#endif // DATABASEMANAGER_H
