#ifndef LOCATION_H
#define LOCATION_H

#include <QObject>

struct Location
{
    Q_GADGET

    Q_PROPERTY(qreal lat MEMBER lat)
    Q_PROPERTY(qreal lon MEMBER lon)

public:
    qreal lat;
    qreal lon;
};
Q_DECLARE_METATYPE(Location)

#endif // LOCATION_H
