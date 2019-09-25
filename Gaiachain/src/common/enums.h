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
        InvalidCommodity = -1,
        Timber = 0,
        Charcoal,
        Cocoa,

        CommodityCount
    };
    Q_ENUM_NS(CommodityType)
    inline uint qHash(CommodityType key, uint seed) {return qH<CommodityType>(key, seed);}

    enum class Page {
        InvalidPage = -1,
        CalendarEvents = 0,     // TODO check if still needed
        CalendarMonth,          // TODO check if still needed
        Calendar,
        EditableEventDetails,   // TODO check if still needed
        EventDetails,           // TODO check if still needed
        EventsList,             // TODO check if still needed
        Login,
        LoginLoading,
        MainMenu,
        QRScanner,              // TODO check if still needed
        ShipmentDetails,        // TODO check if still needed
        Transactions,
        Tracking,
        Map,                    // TODO check if still needed

        PageCount
    };
    Q_ENUM_NS(Page)
    inline uint qHash(Page key, uint seed) {return qH<Page>(key, seed);}

    enum class Popup {
        InvalidPopup = -1,
        Information,

        PopupCount
    };
    Q_ENUM_NS(Popup)
    inline uint qHash(Popup key, uint seed) {return qH<Popup>(key, seed);}

    enum class PageSections {
        DefaultSection = -1,
        ViewTypeSection = 0,
        CalendarSection,
        EditableEventDetailsSection,
        EventsListSection,
        EventDetailsSection,
        ShipmentDetailsSection,
        QRSection,
        HomeSection,

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
        NurseryWorker,
        VillageWorker,
        BaggingWorker,
        TruckDriver,

        UserTypeCount
    };
    Q_ENUM_NS(UserType)
    inline uint qHash(UserType key, uint seed) { return qH<UserType>(key, seed);}

    enum class PlaceType {
        InvalidPlace = -1,
        Forestery,
        LogPark,
        Sawmill,
        Export,
        Nursery,
        Village,
        Bagging,
        Truck,

        PlaceTypeCount
    };
    Q_ENUM_NS(PlaceType)
    inline uint qHash(PlaceType key, uint seed) { return qH<PlaceType>(key, seed);}

    enum class PlaceAction {
        InvalidPlaceAction = -1,
        Arrived,
        Departed,

        PlaceActionCount
    };
    Q_ENUM_NS(PlaceAction)
    inline uint qHash(PlaceAction key, uint seed) { return qH<PlaceAction>(key, seed);}

    enum class PopupAction {
        InvalidPopupAction = -1,
        Accept,
        Cancel,
        Save,
        Logout,
        Exit,
        TryAgain,
        ReloadData,

        PopupActionCount
    };
    Q_ENUM_NS(PopupAction)
    inline uint qHash(PopupAction key, uint seed) { return qH<PopupAction>(key, seed);}

    enum class ConnectionState {
        Unknown = -1,
        ConnectionSuccessful,
        Connecting,
        ConnectionError,

        ConnectionStateCount
    };
    Q_ENUM_NS(ConnectionState)
    inline uint qHash(ConnectionState key, uint seed) { return qH<ConnectionState>(key, seed);}

    // REMEMBER TO REGISTER ENUM IN maincontroller.setupQmlContext TO MAKE IT VISIBLE FOR QML!
}


#endif // ENUMS_H
