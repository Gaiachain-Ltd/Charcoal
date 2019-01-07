#ifndef ENUMS_H
#define ENUMS_H

#include <QObject>

namespace Enums {
    Q_NAMESPACE

    template<typename T>
    inline uint qH(T key, uint seed) {
        return ::qHash(static_cast<int>(key), seed);
    }

    enum class CommodityType {
        Timber = 0,
        Charcoal,
        Cocoa,

        CommodityCount
    };
    Q_ENUM_NS(CommodityType)
    inline uint qHash(CommodityType key, uint seed) {return qH<CommodityType>(key, seed);}

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
        QRScanner,

        PageCount
    };
    Q_ENUM_NS(Page)
    inline uint qHash(Page key, uint seed) {return qH<Page>(key, seed);}

    enum class PageSections {
        DefaultSection = -1,
        ViewTypeSection = 0,
        CalendarSection,
        EventsListSection,
        EventsDetailsSection,
        ShipmentDetailsSection,
        QRSection,

        PageSectionsCount
    };
    Q_ENUM_NS(PageSections)
    inline uint qHash(PageSections key, uint seed) { return qH<PageSections>(key, seed);}
}


#endif // ENUMS_H
