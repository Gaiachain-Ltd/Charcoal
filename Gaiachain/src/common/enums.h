#ifndef ENUMS_H
#define ENUMS_H

#include <QObject>

namespace Enums {
    Q_NAMESPACE

    enum class CommodityType {
        Timber = 0,
        Charcoal,
        Cocoa,
        Count
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
        LanguageChosing,
        Login,
        ResourceChosing,
        ShipmentDetails,
        ViewType,
        Count
    };
    Q_ENUM_NS(Page)
}


#endif // ENUMS_H
