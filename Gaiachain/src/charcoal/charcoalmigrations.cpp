#include <QVector>
#include <QString>
#include <functional>

#include "database/dbmigrations.h"
#include "database/dbhelpers.h"

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
        { 0, 0, 2 },
        std::bind(&Helpers::runQueries, std::placeholders::_1, QList<QLatin1String>{
            // Additional data
            QLatin1String("CREATE TABLE Villages (`id` INTEGER primary key UNIQUE, `name` TEXT NOT NULL)"),
            QLatin1String("CREATE TABLE TreeSpecies (`id` INTEGER primary key UNIQUE, `name` TEXT NOT NULL)"),
            QLatin1String("CREATE TABLE Parcels (`id` INTEGER primary key UNIQUE, `name` TEXT NOT NULL)"),
            QLatin1String("CREATE TABLE Destinations (`id` INTEGER primary key UNIQUE, `name` TEXT NOT NULL)"),
            QLatin1String("CREATE TABLE OvenTypes (`id` INTEGER primary key UNIQUE, `name` TEXT NOT NULL)"),
            // SupplyChain
            QLatin1String("CREATE TABLE Transactions (`id` INTEGER primary key UNIQUE, `name` TEXT NOT NULL)"),
            QLatin1String("CREATE TABLE TransactionTypes (`id` INTEGER primary key UNIQUE, `name` TEXT NOT NULL)"),
            QLatin1String("CREATE TABLE Steps (`id` INTEGER primary key UNIQUE, `date` INTEGER NOT NULL, `locationLat` REAL NOT NULL, `locationLon` REAL NOT NULL, `properties` TEXT NOT NULL)")
        }, true),
        std::bind(&Helpers::runQueries, std::placeholders::_1, QList<QLatin1String>{
            // Additional data
            QLatin1String("DROP TABLE Villages"),
            QLatin1String("DROP TABLE TreeSpecies"),
            QLatin1String("DROP TABLE Parcels"),
            QLatin1String("DROP TABLE Destinations"),
            QLatin1String("DROP TABLE OvenTypes"),
            // Supply chain
            QLatin1String("DROP TABLE Transactions"),
            QLatin1String("DROP TABLE TransactionTypes"),
            QLatin1String("DROP TABLE Steps"),
        }, true)
    },
    // This inserts some static data!
    {
        { 0, 0, 3 },
        std::bind(&Helpers::runQueries, std::placeholders::_1, QList<QLatin1String>{
            // Additional data
            // Nothing static here
            // Supply chain
            QLatin1String("INSERT INTO TransactionTypes (name) VALUES (`Plot`)"),
            QLatin1String("INSERT INTO TransactionTypes (name) VALUES (`Harvest`)"),
            QLatin1String("INSERT INTO TransactionTypes (name) VALUES (`Transport`)"),
        }, true),
        std::bind(&Helpers::runQueries, std::placeholders::_1, QList<QLatin1String>{
            QLatin1String()
        }, true)
    },
    // This inserts some DUMMY data!
    // TODO: REMOVE - use web to get this info
    {
        { 0, 0, 4 },
        std::bind(&Helpers::runQueries, std::placeholders::_1, QList<QLatin1String>{
            // Additional data
            QLatin1String("INSERT INTO Villages (name) VALUES (`Village One`)"),
            QLatin1String("INSERT INTO Villages (name) VALUES (`Village Two`)"),
            // Supply chain
            QLatin1String("INSERT INTO TreeSpecies (name) VALUES (`Cassia siamea`)"),
            QLatin1String("INSERT INTO TreeSpecies (name) VALUES (`Technona grandis`)"),
        }, true),
        std::bind(&Helpers::runQueries, std::placeholders::_1, QList<QLatin1String>{
            QLatin1String()
        }, true)
    },
};
