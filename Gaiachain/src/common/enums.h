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
        InvalidPage = -1,
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
        QrScanner,
        PageCount
    };
    Q_ENUM_NS(Page)

    enum class UserType {
        Producer,
        LogParkWorker,
        SawmillWorker,
        Exporter,

        UserTypeCount
    };
    Q_ENUM_NS(UserType)

    enum class PlaceType {
        Forestery,
        LogPark,
        Sawmill,
        Export,

        PlaceTypeCount
    };
    Q_ENUM_NS(PlaceType)
}


#endif // ENUMS_H
