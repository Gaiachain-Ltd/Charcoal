#pragma once

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QGeoCoordinate>

#include <QDebug>

class QSqlQuery;

class OvenDimensions
{
    Q_GADGET

    Q_PROPERTY(qreal width MEMBER width)
    Q_PROPERTY(qreal length MEMBER length)
    Q_PROPERTY(qreal height1 MEMBER height1)
    Q_PROPERTY(qreal height2 MEMBER height2)
    Q_PROPERTY(bool hasFixedDimensions MEMBER hasFixedDimensions CONSTANT)

public:
    qreal height1 = -1.0;
    qreal height2 = -1.0;
    qreal width = -1.0;
    qreal length = -1.0;
    bool hasFixedDimensions = false;

    Q_INVOKABLE int count() const;
    Q_INVOKABLE qreal volume() const;
};

Q_DECLARE_METATYPE(OvenDimensions)

QDebug operator<<(QDebug debug, const OvenDimensions &dims);

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
    QString carbonizerId;
    OvenDimensions dimensions;

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

    QVariantList qrCodes() const;
};

struct Entity {
    int id = -1;
    int parent = -1;
    int typeId = -1;
    QString name;

    bool loadFromDb(const QString &connectionName, const int id);
    QVector<Event> loadEvents(const QString &connectionName) const;
};

