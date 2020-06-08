#include "entitiesmodel.h"

#include "database/dbhelpers.h"
#include "common/logs.h"

#include <QGeoCoordinate>

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

#include <QMetaEnum>
#include <QDateTime>
#include <QDate>

EntitiesModel::EntitiesModel(QObject *parent) : QSqlQueryModel(parent)
{
}

void EntitiesModel::setDbConnection(/*const QString &path, */const QString &connectionName)
{

    m_dbConnName = connectionName;

    setQuery("SELECT name FROM Entities", db::Helpers::databaseConnection(connectionName));

    // TODO: we need to also return Events for each Entity!
}

QString EntitiesModel::generatePlotId(const QString &userId,
                                      const QString &parcelCode,
                                      const QDate &date) const
{
    return userId + sep + parcelCode + sep + date.toString(dateFormat);
}

QString EntitiesModel::generateHarvestId(const QString &plotId,
                                         const QString &userId) const
{
    return plotId + sep + userId;
}

QString EntitiesModel::generateTransportId(const QString &harvestId,
                                           const QString &licensePlate,
                                           const int transportNumber,
                                           const QDate &date) const
{
    return harvestId + sep + licensePlate
        + sep + "T" + QString::number(transportNumber)
        + sep + date.toString(dateFormat);
}

void EntitiesModel::registerLoggingBeginning(
    const QGeoCoordinate &coordinate,
    const QDateTime &timestamp, const QString &parcel,
    const QString &malebiRepsId, const QString &village,
    const QString &treeSpecies)
{
    /*
     * Algorithm is:
     * - insert entity into table
     * - get new entity's ID
     * - insert event into table
     * - send action to web server
     */

    qDebug() << "Registering logging beginning" << coordinate << timestamp
             << parcel << malebiRepsId << village << treeSpecies;

    // First, insert a new Entity into table
    const QString plotId(generatePlotId(malebiRepsId, parcel, timestamp.date()));
    const QString typeId(getEntityTypeId(Enums::PackageType::Plot));

    if (typeId.isEmpty()) {
        qWarning() << RED("Plot ID type not found!");
        return;
    }

    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));

    query.prepare("INSERT INTO Entities (typeId, name, isUsed, isCommitted) "
                  "VALUES (:typeId, :plotId, 0, 0)");
    query.bindValue(":typeId", typeId);
    query.bindValue(":plotId", plotId);

    if (query.exec() == false) {
        qWarning() << RED("Inserting Plot entity has failed!")
                   << query.lastError().text()
                   << "for query:" << query.lastQuery()
                   << "DB:" << m_dbConnName;
        return;
    }

    // Then, insert a new Event under that Entity
    const QString entityId(query.lastInsertId().toString());
    const QString eventTypeId(getEventTypeId(Enums::SupplyChainAction::LoggingBeginning));

    if (eventTypeId.isEmpty()) {
        qWarning() << RED("Event Type ID not found!");
        return;
    }

    query.prepare("INSERT INTO Events (entityId, typeId, "
                  "date, locationLatitude, locationLongitude, properties) "
                  "VALUES (:entityId, :typeId, :date, :locationLatitude, :locationLongitude,"
                  ":properties)");
    query.bindValue(":entityId", entityId);
    query.bindValue(":typeId", eventTypeId);
    query.bindValue(":date", timestamp);
    query.bindValue(":locationLatitude", coordinate.latitude());
    query.bindValue(":locationLongitude", coordinate.longitude());
    // TODO: use Tags to denote the properties more reliably!
    query.bindValue(":properties", QVariantMap {
                                       { "parcel", parcel },
                                       { "village", village },
                                       { "treeSpecies", treeSpecies },
                                       { "userId", malebiRepsId }
                                   });

    if (query.exec() == false) {
        qWarning() << RED("Inserting Logging Beginning event has failed!")
                   << query.lastError().text() << "for query:" << query.lastQuery();
        return;
    }

    // Lastly, send a request to server to add it, too.
}

QString EntitiesModel::getEntityTypeId(const Enums::PackageType type) const
{
    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));
    const QString typeString(QMetaEnum::fromType<Enums::PackageType>()
                                 .valueToKey(int(type)));

    query.prepare("SELECT id FROM EntityTypes WHERE name=:typeString");
    query.bindValue(":typeString", typeString);

    if (query.exec()) {
        qDebug() << type << typeString << query.record();
        query.next();
        return query.value("id").toString();
    }

    qWarning() << RED("Getting EntityType has failed!")
               << query.lastError().text()
               << "for query:" << query.lastQuery()
               << "DB:" << m_dbConnName;

    return QString();
}

QString EntitiesModel::getEventTypeId(const Enums::SupplyChainAction action) const
{
    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));
    const QString typeString(getActionAbbreviation(action));

    query.prepare("SELECT id FROM EventTypes WHERE actionName=:typeString");
    query.bindValue(":typeString", typeString);

    if (query.exec()) {
        qDebug() << action << typeString << query.record();
        query.next();
        return query.value("id").toString();
    }

    qWarning() << RED("Getting EventType has failed!")
               << query.lastError().text()
               << "for query:" << query.lastQuery()
               << "DB:" << m_dbConnName;

    return QString();
}

QString EntitiesModel::getActionAbbreviation(const Enums::SupplyChainAction action) const
{
    switch (action) {
    case Enums::SupplyChainAction::LoggingBeginning:
        return QStringLiteral("LB");
    case Enums::SupplyChainAction::LoggingEnding:
        return QStringLiteral("LE");
    case Enums::SupplyChainAction::CarbonizationBeginning:
        return QStringLiteral("CB");
    case Enums::SupplyChainAction::CarbonizationEnding:
        return QStringLiteral("CE");
    case Enums::SupplyChainAction::LoadingAndTransport:
        return QStringLiteral("TR");
    case Enums::SupplyChainAction::Reception:
        return QStringLiteral("RE");
    default: return QString();
    }

    return QString();
}
