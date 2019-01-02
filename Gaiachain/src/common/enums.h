#ifndef ENUMS_H
#define ENUMS_H

#include <QObject>

namespace Enums {
    Q_NAMESPACE

    enum class CommodityType {
        Timber = 0,
        Charcoal,
        Cocoa,
        CommodityCount
    };
    Q_ENUM_NS(CommodityType)

    inline uint qHash(CommodityType key, uint seed)
    {
        return ::qHash(static_cast<int>(key), seed);
    }

    enum class Page {
        CalendarEvents = 0,
        CalendarMonth,
        Calendar,
        EventDetails,
        EventsList,
        LanguageChosing,
        Login,
        ResourceChosing,
        ShipmentDetails,
        ViewType,
        PageCount
    };
    Q_ENUM_NS(Page)
}


#endif // ENUMS_H
