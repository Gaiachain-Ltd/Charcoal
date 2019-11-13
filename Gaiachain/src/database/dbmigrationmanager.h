#ifndef MIGRATIONMANAGER_H
#define MIGRATIONMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QVersionNumber>
#include <functional>

#include "dbmigrations.h"

namespace db {

class MigrationManager final : public QObject
{
    Q_OBJECT
public:
    MigrationManager(QSqlDatabase &db, QObject *parent = nullptr);

    bool checkAndCreate();

    bool needsUpdate();
    bool update();

private:
    static const QLatin1String sc_dbName;
    const QString c_dbPath;

    QSqlDatabase &m_db;
    QVersionNumber m_dbVersion;

    bool dbExist() const;
    bool openDb();

    QVersionNumber getVersionNumber() const;
    bool updateDb();

    template<typename It>
    It findMigrationNumber(It begin, It end, const QVersionNumber &number);

    template<typename It>
    bool applyMigrations(It begin, It end, std::function<bool(const Migration &)> const &handler, bool forward);
};

}
#endif // MIGRATIONMANAGER_H
