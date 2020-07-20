#include "dbobjects.h"

#include "common/logs.h"
#include "common/tags.h"
#include "database/dbhelpers.h"

#include <QSqlQuery>
#include <QSqlError>

#include <QJsonDocument>

void Oven::updateDates(const Oven &other, const Event &otherEvent)
{
    if (other.carbonizationBeginningId != -1) {
        carbonizationBeginningId = other.carbonizationBeginningId;
        carbonizationBeginning = otherEvent.date;
    }

    if (other.carbonizationEndingId != -1) {
        carbonizationEndingId = other.carbonizationEndingId;
        carbonizationEnding = otherEvent.date;
    }
}

Oven Event::loadOven(const QString &connectionName) const
{
    QSqlQuery q(QString(), db::Helpers::databaseConnection(connectionName));
    q.prepare("SELECT id, type, plot, "
              "carbonizationBeginning, carbonizationEnding, "
              "name, "
              "oven_height, oven_width, oven_length "
              "FROM Ovens "
              "WHERE carbonizationBeginning=:id OR carbonizationEnding=:id");
    q.bindValue(":id", id);

    if (q.exec() == false) {
        qWarning() << RED("Getting oven details has failed!")
                   << q.lastError().text() << "for query:" << q.lastQuery() << id;
        return {};
    }

    if (q.next() == false) {
        return {};
    }

    Oven oven;
    oven.id = q.value("id").toInt();
    oven.typeId = q.value("type").toInt();
    oven.plotId = q.value("plot").toInt();
    oven.name = q.value("name").toString();
    oven.height = q.value("oven_height").toReal();
    oven.width = q.value("oven_width").toReal();
    oven.length = q.value("oven_length").toReal();
    oven.carbonizerId = userId;

    const QVariant beginningEvent(q.value("carbonizationBeginning"));
    if (beginningEvent.isValid() && beginningEvent.toInt() == id) {
        oven.carbonizationBeginningId = id;
        oven.carbonizationBeginning = date;
    }

    const QVariant endingEvent(q.value("carbonizationEnding"));
    if (endingEvent.isValid() && endingEvent.toInt() == id) {
        oven.carbonizationEndingId = id;
        oven.carbonizationEnding = date;
    }

    return oven;
}

void Event::loadFromQuery(QSqlQuery *query)
{
    id = query->value("id").toInt();
    typeId = query->value("typeId").toInt();
    entityId = query->value("entityId").toInt();
    userId = query->value("userId").toString();
    date = query->value("eventDate").toLongLong();
    timestamp = query->value("date").toLongLong();

    const double latitude = query->value("locationLatitude").toDouble();
    const double longitude = query->value("locationLongitude").toDouble();
    location = QGeoCoordinate(latitude, longitude);

    properties = QJsonDocument::fromJson(
                     query->value(Tags::properties).toByteArray()).object();
}

bool Entity::loadFromDb(const QString &connectionName, const int id)
{
    QSqlQuery q(QString(), db::Helpers::databaseConnection(connectionName));
    q.prepare("SELECT id, typeId, name, parent, webId FROM Entities "
              "WHERE id=:id");
    q.bindValue(":id", id);

    if (q.exec() && q.next()) {
        this->id = q.value("id").toInt();
        parent = q.value("parent").toInt();
        typeId = q.value("typeId").toInt();
        name = q.value("name").toString();
        return true;
    } else {
        qWarning() << RED("Could not load Entity from DB")
                   << "SQL error:" << q.lastError()
                   << "For query:" << q.lastQuery()
                   << id;
    }

    return false;
}

QVector<Event> Entity::loadEvents(const QString &connectionName) const
{
    QSqlQuery q(QString(), db::Helpers::databaseConnection(connectionName));
    q.prepare("SELECT id, entityId, typeId, userId, eventDate, date, "
              "locationLatitude, locationLongitude, properties "
              "FROM Events "
              "WHERE entityId=:id");
    q.bindValue(":id", id);

    if (q.exec() == false) {
        qWarning() << RED("Getting event details has failed!")
                   << q.lastError().text() << "for query:" << q.lastQuery();
        return {};
    }

    QVector<Event> events;
    while (q.next()) {
        Event event;
        event.loadFromQuery(&q);
        events.append(event);
    }

    return events;
}
