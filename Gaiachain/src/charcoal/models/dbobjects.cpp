#include "dbobjects.h"

#include "common/logs.h"
#include "common/tags.h"
#include "database/dbhelpers.h"
#include "charcoal/database/charcoaldbhelpers.h"

#include <QSqlQuery>
#include <QSqlError>

#include <QJsonDocument>
#include <QJsonArray>

int OvenDimensions::count() const
{
    if (height2 < 0 || qFuzzyIsNull(height2)) {
        return 3;
    } else {
        return 4;
    }
}

qreal OvenDimensions::volume() const
{
    return CharcoalDbHelpers::ovenVolume(width, length, height1, height2);
}

QDebug operator<<(QDebug debug, const OvenDimensions &dims)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << "( w: " << dims.width << ", l: " << dims.length
                    << ", h1: " << dims.height1
                    << ", h2: " << dims.height2
                    << ')';

    return debug;
}

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
              "oven_height, oven_height2, oven_width, oven_length "
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
    oven.id = q.value(Tags::id).toInt();
    oven.typeId = q.value(Tags::type).toInt();
    oven.plotId = q.value(Tags::plot).toInt();
    oven.name = q.value(Tags::name).toString();
    oven.dimensions.height1 = q.value(Tags::webOvenHeight).toReal();
    oven.dimensions.height2 = q.value(Tags::webOvenHeight2).toReal();
    if (qFuzzyIsNull(oven.dimensions.height2)) {
        oven.dimensions.height2 = -1;
    }
    oven.dimensions.width = q.value(Tags::webOvenWidth).toReal();
    oven.dimensions.length = q.value(Tags::webOvenLength).toReal();
    oven.carbonizerId = userId;

    const QVariant beginningEvent(q.value(Tags::carbonizationBeginning));
    if (beginningEvent.isValid() && beginningEvent.toInt() == id) {
        oven.carbonizationBeginningId = id;
        oven.carbonizationBeginning = date;
    }

    const QVariant endingEvent(q.value(Tags::carbonizationEnding));
    if (endingEvent.isValid() && endingEvent.toInt() == id) {
        oven.carbonizationEndingId = id;
        oven.carbonizationEnding = date;
    }

    return oven;
}

void Event::loadFromQuery(QSqlQuery *query)
{
    id = query->value(Tags::id).toInt();
    typeId = query->value(Tags::typeId).toInt();
    entityId = query->value(Tags::entityId).toInt();
    userId = query->value("userId").toString();
    date = query->value(Tags::eventDate).toLongLong();
    timestamp = query->value(Tags::date).toLongLong();

    const double latitude = query->value(Tags::locationLatitude).toDouble();
    const double longitude = query->value(Tags::locationLongitude).toDouble();
    location = QGeoCoordinate(latitude, longitude);

    properties = QJsonDocument::fromJson(
                query->value(Tags::properties).toByteArray()).object();
}

QVariantList Event::qrCodes() const
{
    return properties.value(Tags::webQrCodes).toArray().toVariantList();
}

bool Entity::loadFromDb(const QString &connectionName, const int id)
{
    QSqlQuery q(QString(), db::Helpers::databaseConnection(connectionName));
    q.prepare("SELECT id, typeId, name, parent, webId FROM Entities "
              "WHERE id=:id");
    q.bindValue(":id", id);

    if (q.exec() && q.next()) {
        this->id = q.value(Tags::id).toInt();
        parent = q.value(Tags::parent).toInt();
        typeId = q.value(Tags::typeId).toInt();
        name = q.value(Tags::name).toString();
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
              "WHERE entityId=:id "
              "ORDER BY eventDate DESC");
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
