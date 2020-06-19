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
        { 0, 9, 1 },
        std::bind(&Helpers::runQueries, std::placeholders::_1, QList<QLatin1String>{
            QLatin1String("CREATE TABLE Producers   (`id` 		INTEGER primary key UNIQUE,"
                                                    "`code`     TEXT NOT NULL,"
                                                    "`name`		TEXT NOT NULL,"
                                                    "`village`  TEXT NOT NULL"
                                                    ")"),
            QLatin1String("CREATE TABLE Parcels (`id`           INTEGER primary key UNIQUE,"
                                                "`code`         TEXT NOT NULL,"
                                                "`producerId`	INTEGER NOT NULL,"
                                                "FOREIGN KEY(`producerId`) REFERENCES `Producers`(`id`)"
                                                ")"),
            QLatin1String("CREATE TABLE Companies   (`id` 	INTEGER primary key UNIQUE,"
                                                    "`name`	TEXT NOT NULL,"
                                                    "`code` TEXT NOT NULL,"
                                                    "`type`	INTEGER NOT NULL"
                                                    ")"),
            QLatin1String("CREATE TABLE Destinations    (`id` 	INTEGER primary key UNIQUE,"
                                                        "`name`	TEXT NOT NULL"
                                                        ")"),
            QLatin1String("CREATE TABLE Events  (`id` 				INTEGER primary key AUTOINCREMENT,"
                                                "`packageId`		TEXT NOT NULL,"
                                                "`action`			INTEGER NOT NULL,"
                                                "`timestamp`		INTEGER NOT NULL,"
                                                "`cooperativeId`	INTEGER NOT NULL,"
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
        }, true),
        std::bind(&Helpers::runQueries, std::placeholders::_1, QList<QLatin1String>{
            QLatin1String("DROP TABLE Producers"),
            QLatin1String("DROP TABLE Parcels"),
            QLatin1String("DROP TABLE Companies"),
            QLatin1String("DROP TABLE Destinations"),
            QLatin1String("DROP TABLE Events"),
            QLatin1String("DROP TABLE Relations"),
            QLatin1String("DROP TABLE UnusedIds"),
        }, true)
    },
};
