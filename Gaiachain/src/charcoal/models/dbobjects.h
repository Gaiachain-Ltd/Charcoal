#pragma once

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QGeoCoordinate>

class QSqlQuery;

struct Event;
struct Oven {
    int id = -1;
    int typeId = -1;
    int plotId = -1;
    int carbonizationBeginningId = -1;
    int carbonizationEndingId = -1;
    qint64 carbonizationBeginning = -1;
    qint64 carbonizationEnding = -1;
    QString name;
    float height1 = -1.0;
    float height2 = -1.0;
    float width = -1.0;
    float length = -1.0;

    QString carbonizerId;

    float volume() const;

    void updateDates(const Oven &other, const Event &otherEvent);
};

struct Event {
    int id = -1;
    int typeId = -1;
    int entityId = -1;
    QString userId;
    qint64 timestamp = -1;
    qint64 date = -1;
    QGeoCoordinate location;
    QJsonObject properties;

    Oven loadOven(const QString &connectionName) const;
    void loadFromQuery(QSqlQuery *query);
};

struct Entity {
    int id = -1;
    int parent = -1;
    int typeId = -1;
    QString name;

    bool loadFromDb(const QString &connectionName, const int id);
    QVector<Event> loadEvents(const QString &connectionName) const;
};

