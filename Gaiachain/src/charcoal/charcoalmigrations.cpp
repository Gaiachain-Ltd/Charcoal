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
QLatin1String("CREATE TABLE Plots (`id` INTEGER primary key UNIQUE, `name` TEXT NOT NULL)"),
QLatin1String("CREATE TABLE Harvests (`id` INTEGER primary key UNIQUE, `name` TEXT NOT NULL)"),
QLatin1String("CREATE TABLE Transports (`id` INTEGER primary key UNIQUE, `name` TEXT NOT NULL)")
        }, true),
        std::bind(&Helpers::runQueries, std::placeholders::_1, QList<QLatin1String>{
            QLatin1String("DROP TABLE Villages"),
            QLatin1String("DROP TABLE TreeSpecies"),
            QLatin1String("DROP TABLE Parcels"),
            QLatin1String("DROP TABLE Destinations"),
            QLatin1String("DROP TABLE OvenTypes"),
            QLatin1String("DROP TABLE Plots"),
            QLatin1String("DROP TABLE Harvests"),
            QLatin1String("DROP TABLE Transports"),
        }, true)
    },
};
