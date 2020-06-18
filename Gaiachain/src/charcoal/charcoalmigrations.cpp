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
            QLatin1String("CREATE TABLE Villages (`id` INTEGER primary key AUTOINCREMENT, `name` TEXT NOT NULL)"),
            QLatin1String("CREATE TABLE TreeSpecies (`id` INTEGER primary key AUTOINCREMENT, `name` TEXT NOT NULL)"),
            QLatin1String("CREATE TABLE Parcels (`id` INTEGER primary key AUTOINCREMENT, `name` TEXT NOT NULL)"),
            QLatin1String("CREATE TABLE Destinations (`id` INTEGER primary key AUTOINCREMENT, `name` TEXT NOT NULL)"),
            QLatin1String("CREATE TABLE OvenTypes (`id` INTEGER primary key AUTOINCREMENT, `name` TEXT NOT NULL, `height` DECIMAL(5,2), `length` DECIMAL(5,2), `width` DECIMAL(5,2))"),

            // SupplyChain
            // Entities are how Transactions are called on Web side
            // "name" is ID - PlotId, HarvestId, TransportId
            // "parent" is the ID of plot Entity which is the parent of whole supply chain operation
            QLatin1String("CREATE TABLE Entities "
                "(`id` INTEGER primary key AUTOINCREMENT, "
                "`typeId` INTEGER NOT NULL, `name` TEXT NOT NULL, `parent` INTEGER, "
                "`isFinished` BOOLEAN NOT NULL CHECK (isFinished IN (0,1)), "
                "`isCommitted` BOOLEAN NOT NULL CHECK (isCommitted IN (0,1)), "
                "`isReplanted` BOOLEAN NOT NULL CHECK (isReplanted IN (0,1)), "
                "FOREIGN KEY(typeId) REFERENCES EntityTypes(id), "
                "FOREIGN KEY(parent) REFERENCES Entities(id))"),
            QLatin1String("CREATE TABLE EntityTypes (`id` INTEGER primary key AUTOINCREMENT, `name` TEXT NOT NULL)"),
            QLatin1String("CREATE TABLE Events "
                "(`id` INTEGER primary key AUTOINCREMENT, `entityId` INTEGER NOT NULL, "
                "`typeId` INTEGER NOT NULL, `userId` TEXT NOT NULL, `date` INTEGER NOT NULL, "
                "`locationLatitude` REAL NOT NULL, `locationLongitude` REAL NOT NULL, "
                "`properties` TEXT NOT NULL, "
                "FOREIGN KEY(entityId) REFERENCES Entities(id), "
                "FOREIGN KEY(typeId) REFERENCES EventTypes(id))"),
            QLatin1String("CREATE TABLE EventTypes (`id` INTEGER primary key AUTOINCREMENT, `actionName` TEXT NOT NULL)"),
            QLatin1String("CREATE TABLE Replantations (`id` INTEGER primary key AUTOINCREMENT, "
                "`plotId` INTEGER NOT NULL, `userId` TEXT NOT NULL, "
                "`numberOfTrees` INTEGER NOT NULL, `treeSpecies` INTEGER NOT NULL, "
                "`locationLatitude` REAL NOT NULL, `locationLongitude` REAL NOT NULL, "
                "`beginningDate` INTEGER NOT NULL, `endingDate` INTEGER NOT NULL, "
                "FOREIGN KEY(plotId) REFERENCES Entities(id), "
                "FOREIGN KEY(treeSpecies) REFERENCES TreeSpecies(id))"),
            QLatin1String("CREATE TABLE Ovens (`id` INTEGER primary key AUTOINCREMENT, "
                "`type` INTEGER NOT NULL, `plot` INTEGER NOT NULL, "
                "`carbonizationBeginning` INTEGER NOT NULL, `carbonizationEnding` INTEGER, "
                "`name` TEXT NOT NULL, "
                "`height` DECIMAL(5,2), `length` DECIMAL(5,2), `width` DECIMAL(5,2), "
                "FOREIGN KEY(type) REFERENCES OvenTypes(id), "
                "FOREIGN KEY(plot) REFERENCES Entities(id), "
                "FOREIGN KEY(carbonizationBeginning) REFERENCES Events(id), "
                "FOREIGN KEY(carbonizationEnding) REFERENCES Events(id))")
        }, true),
        std::bind(&Helpers::runQueries, std::placeholders::_1, QList<QLatin1String>{
            // Additional data
            QLatin1String("DROP TABLE Villages"),
            QLatin1String("DROP TABLE TreeSpecies"),
            QLatin1String("DROP TABLE Parcels"),
            QLatin1String("DROP TABLE Destinations"),
            QLatin1String("DROP TABLE OvenTypes"),
            // Supply chain
            QLatin1String("DROP TABLE Entities"),
            QLatin1String("DROP TABLE EntityTypes"),
            QLatin1String("DROP TABLE Events"),
            QLatin1String("DROP TABLE EventTypes"),
            QLatin1String("DROP TABLE Replantations"),
            QLatin1String("DROP TABLE Ovens"),
        }, true)
    },
    // This inserts some static data!
    {
        { 0, 0, 3 },
        std::bind(&Helpers::runQueries, std::placeholders::_1, QList<QLatin1String>{
            // Additional data
            // Nothing static here
            // Supply chain
            QLatin1String("INSERT INTO EntityTypes (name) VALUES (\"Plot\")"),
            QLatin1String("INSERT INTO EntityTypes (name) VALUES (\"Harvest\")"),
            QLatin1String("INSERT INTO EntityTypes (name) VALUES (\"Transport\")"),

            // action names are same as Web API actions, to make it simple
            QLatin1String("INSERT INTO EventTypes (actionName) VALUES (\"LB\")"), // Logging beginning
            QLatin1String("INSERT INTO EventTypes (actionName) VALUES (\"LE\")"), // Logging ending
            QLatin1String("INSERT INTO EventTypes (actionName) VALUES (\"CB\")"), // Carbonization beginning
            QLatin1String("INSERT INTO EventTypes (actionName) VALUES (\"CE\")"), // Carbonization ending
            QLatin1String("INSERT INTO EventTypes (actionName) VALUES (\"TR\")"), // Loading & transport
            QLatin1String("INSERT INTO EventTypes (actionName) VALUES (\"RE\")"), // Reception
        }, true),
        std::bind(&Helpers::runQueries, std::placeholders::_1, QList<QLatin1String>{
            QLatin1String("DELETE FROM EntityTypes"),
            QLatin1String("DELETE FROM EventTypes"),
            QLatin1String("VACUUM")
        }, true)
    },
    // This inserts some DUMMY const data!
    // TODO: REMOVE - use web to get this info
    {
        { 0, 0, 4 },
        std::bind(&Helpers::runQueries, std::placeholders::_1, QList<QLatin1String>{
            // Additional data
            QLatin1String("INSERT INTO Villages (name) VALUES (\"Village One\")"),
            QLatin1String("INSERT INTO Villages (name) VALUES (\"Village Two\")"),
            QLatin1String("INSERT INTO Villages (name) VALUES (\"Village Three\")"),

            QLatin1String("INSERT INTO TreeSpecies (name) VALUES (\"Cassia siamea\")"),
            QLatin1String("INSERT INTO TreeSpecies (name) VALUES (\"Technona grandis\")"),

            QLatin1String("INSERT INTO Parcels (name) VALUES (\"0595112\")"),
            QLatin1String("INSERT INTO Parcels (name) VALUES (\"1234567\")"),
            QLatin1String("INSERT INTO Parcels (name) VALUES (\"9955112\")"),
            QLatin1String("INSERT INTO Parcels (name) VALUES (\"1100110\")"),

            QLatin1String("INSERT INTO Destinations (name) VALUES (\"Abidjan\")"),

            QLatin1String("INSERT INTO OvenTypes (name) VALUES (\"traditional\")"),
            QLatin1String("INSERT INTO OvenTypes (name, height, length, width) VALUES (\"metallic\", 4, 5, 6)"),
            // Supply chain
        }, true),
        std::bind(&Helpers::runQueries, std::placeholders::_1, QList<QLatin1String>{
            QLatin1String("DELETE FROM Villages"),
            QLatin1String("DELETE FROM TreeSpecies"),
            QLatin1String("DELETE FROM Parcels"),
            QLatin1String("DELETE FROM Destinations"),
            QLatin1String("DELETE FROM OvenTypes"),
            QLatin1String("VACUUM")
        }, true)
    },
};
