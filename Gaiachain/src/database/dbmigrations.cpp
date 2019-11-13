#include "dbmigrations.h"

#include <QSqlQuery>
#include <QDateTime>

#include "dbhelpers.h"

Q_LOGGING_CATEGORY(databaseMigration, "database.migration")

using namespace db;

//!!! Add migrations here
const QVector<Migration> db::DB_MIGRATIONS = {
    {
        { 0, 0, 1 },
        std::bind(&Helpers::runQueries, std::placeholders::_1, QList<QLatin1String>{
            QLatin1String("CREATE TABLE Migrations (`id` INTEGER primary key AUTOINCREMENT, `timestamp` INTEGER NOT NULL, `version` TEXT NOT NULL)"),
        }, true),
        std::bind(&Helpers::runQueries, std::placeholders::_1, QList<QLatin1String>{
            QLatin1String("DROP TABLE Migrations"),
        }, true)
    },
    {
        { 0, 9, 1 },
        std::bind(&Helpers::runQueries, std::placeholders::_1, QList<QLatin1String>{
            QLatin1String("CREATE TABLE Events  (`id` 				INTEGER primary key AUTOINCREMENT,"
                                                "`packageId`		TEXT NOT NULL,"
                                                "`action`			INTEGER NOT NULL,"
                                                "`timestamp`		INTEGER NOT NULL,"
                                                "`userRole`			INTEGER NOT NULL,"
                                                "`cooperativeId`	TEXT NOT NULL,"
                                                "`properties`		TEXT NOT NULL,"
                                                "`locationLat`		REAL NOT NULL,"
                                                "`locationLon`		REAL NOT NULL,"
                                                "`isLocal`			BOOLEAN DEFAULT false,"
                                                "`lastUsed`			INTEGER,"
                                                "UNIQUE(packageId, action)"
                                                ")"),
            QLatin1String("CREATE TABLE Relations   (`id` 			INTEGER primary key AUTOINCREMENT,"
                                                    "`packageId`	TEXT NOT NULL,"
                                                    "`relatedId`	TEXT NOT NULL,"
                                                    "`lastUsed`		INTEGER,"
                                                    "UNIQUE(packageId, relatedId)"
                                                    ")"),
            QLatin1String("CREATE TABLE UnusedIds   (`id` 			INTEGER primary key AUTOINCREMENT,"
                                                    "`packageId`	TEXT NOT NULL UNIQUE,"
                                                    "`packageType`	INTEGER NOT NULL"
                                                    ")"),
            QLatin1String("CREATE TABLE Producers   (`id` 			INTEGER primary key AUTOINCREMENT,"
                                                    "`producerId`	TEXT NOT NULL UNIQUE,"
                                                    "`name`			TEXT NOT NULL,"
                                                    "`village`		TEXT NOT NULL,"
                                                    "`parcelCodes`	TEXT NOT NULL"
                                                    ")"),
            QLatin1String("CREATE TABLE Buyers  (`id` 	INTEGER primary key AUTOINCREMENT,"
                                                "`name`	TEXT NOT NULL UNIQUE"
                                                ")"),
            QLatin1String("CREATE TABLE Transporters    (`id` 	INTEGER primary key AUTOINCREMENT,"
                                                        "`name`	TEXT NOT NULL UNIQUE"
                                                        ")"),
            QLatin1String("CREATE TABLE Destinations    (`id` 	INTEGER primary key AUTOINCREMENT,"
                                                        "`name`	TEXT NOT NULL UNIQUE"
                                                        ")"),
        }, true),
        std::bind(&Helpers::runQueries, std::placeholders::_1, QList<QLatin1String>{
            QLatin1String("DROP TABLE Events"),
            QLatin1String("DROP TABLE Relations"),
            QLatin1String("DROP TABLE UnusedIds"),
            QLatin1String("DROP TABLE Producers"),
            QLatin1String("DROP TABLE Buyers"),
            QLatin1String("DROP TABLE Transporters"),
            QLatin1String("DROP TABLE Destinations"),
        }, true)
    },
};


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
