#include "entitiesmodel.h"

#include "database/dbhelpers.h"
#include "common/logs.h"

#include <QGeoCoordinate>
#include <QSqlQuery>
#include <QSqlError>
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
    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_dbConnName));

    query.prepare("INSERT INTO Entities (name, isUsed, isCommitted) "
                  "VALUES (:plotId, 0, 0)");
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
    query.prepare("INSERT INTO Events (entityId, date, locationLatitude, "
                  "locationLongitude, properties) "
                  "VALUES (:entityId, :date, :locationLatitude, :locationLongitude,"
                  ":properties)");
    query.bindValue(":entityId", entityId);
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
