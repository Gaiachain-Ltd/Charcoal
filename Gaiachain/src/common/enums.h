#ifndef ENUMS_H
#define ENUMS_H

#include <QObject>

namespace Enums {
    Q_NAMESPACE

    template<typename T>
    inline uint qH(T key, uint seed) {
        return ::qHash(static_cast<int>(key), seed);
    }

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

    // TODO update with real values
    enum class UserType {
        NotLoggedUser = -1,
        Producer,
        BaggingPerson,
        Storekeeper,
        Exporter,

        UserTypeCount
    };
    Q_ENUM_NS(UserType)
    inline uint qHash(UserType key, uint seed) { return qH<UserType>(key, seed);}

    // TODO update with real values
    enum class PlaceType {
        InvalidPlace = -1,
        Nursery,
        Village,
        Bagging,
        Truck,
        Export,

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

    enum class SupplyChainAction {
        Unknown = -1,
        Harvest,
        Fermentation,
        Bagging,
        StorageArrival,
        StorageLot,
        Transport,
        Reception,

        ConnectionStateCount
    };
    Q_ENUM_NS(SupplyChainAction)
    inline uint qHash(SupplyChainAction key, uint seed) { return qH<SupplyChainAction>(key, seed);}

    enum class PackageType {
        Unknown = -1,
        Harvest,
        Sac,
        Lot,

        ConnectionStateCount
    };
    Q_ENUM_NS(PackageType)
    inline uint qHash(PackageType key, uint seed) { return qH<PackageType>(key, seed);}

    // REMEMBER TO REGISTER ENUM IN maincontroller.setupQmlContext TO MAKE IT VISIBLE FOR QML!
}


#endif // ENUMS_H
