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
        EditableEventDetails,
        EventDetails,
        EventsList,
        LanguageChosing,
        Login,
        ResourceChosing,
        ShipmentDetails,
        ViewType,
        QRScanner,

        InformationPopup,

        PageCount
    };
    Q_ENUM_NS(Page)
    inline uint qHash(Page key, uint seed) {return qH<Page>(key, seed);}

    enum class PageSections {
        DefaultSection = -1,
        ViewTypeSection = 0,
        CalendarSection,
        EditableEventDetailsSection,
        EventsListSection,
        EventDetailsSection,
        ShipmentDetailsSection,
        QRSection,

        PageSectionsCount
    };
    Q_ENUM_NS(PageSections)
    inline uint qHash(PageSections key, uint seed) { return qH<PageSections>(key, seed);}

    enum class UserType {
        NotLoggedUser = -1,
        Producer,
        LogParkWorker,
        SawmillWorker,
        Exporter,

        UserTypeCount
    };
    Q_ENUM_NS(UserType)
    inline uint qHash(UserType key, uint seed) { return qH<UserType>(key, seed);}

    struct UserTypeStruct {
        static UserType userTypeFromString(const QString &text)
        {
            if (text == QStringLiteral("PRODUCER")) {
                return UserType::Producer;
            } else if (text == QStringLiteral("LOG_PARK")) {
                return UserType::LogParkWorker;
            } else if (text == QStringLiteral("SAWMILL")) {
                return UserType::SawmillWorker;
            } else if (text == QStringLiteral("EXPORTER")) {
                return UserType::Exporter;
            } else {
                return UserType::NotLoggedUser;
            }
        }
    };

    enum class PlaceType {
        InvalidPlace = -1,
        Forestery,
        LogPark,
        Sawmill,
        Export,

        PlaceTypeCount
    };
    Q_ENUM_NS(PlaceType)
    inline uint qHash(PlaceType key, uint seed) { return qH<PlaceType>(key, seed);}

    // REMEMBER TO REGISTER ENUM IN maincontroller.setupQmlContext TO MAKE IT VISIBLE FOR QML!
}


#endif // ENUMS_H
