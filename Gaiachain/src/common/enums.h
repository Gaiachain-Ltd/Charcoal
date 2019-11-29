#ifndef ENUMS_H
#define ENUMS_H

#include <QObject>

//! All enums should:
//! 1. Start with Unknown / Invalid entry of value -1
//! 2. End with EnumCount entry
//! 3. Include consecutive number between

namespace Enums {
    Q_NAMESPACE

    template<typename T>
    inline uint qH(T key, uint seed) {
        return ::qHash(static_cast<int>(key), seed);
    }

    enum class Edge {
        Unknown = -1,
        LeftEdge,
        RightEdge
    };
    Q_ENUM_NS(Edge)
    inline uint qHash(Edge key, uint seed) { return qH<Edge>(key, seed);}

    enum class Page {
        InvalidPage = -1,
        CalendarEvents,
        CalendarYear,
        Calendar,
        Login,
        LoginLoading,
        MainMenu,
        QRScanner,              // TODO check if still needed
        Transactions,
        Tracking,
        PackageData,
        SupplyChainHarvest,
        SupplyChainGrainProcessing,
        WaitingTransactions,

        PageCount
    };
    Q_ENUM_NS(Page)
    inline uint qHash(Page key, uint seed) {return qH<Page>(key, seed);}

    enum class Popup {
        InvalidPopup = -1,
        Information,
        YesNoQuestion,
        WaitOverlay,
        Notification,
        Confirm,

        PopupCount
    };
    Q_ENUM_NS(Popup)
    inline uint qHash(Popup key, uint seed) {return qH<Popup>(key, seed);}

    enum class PopupAction {
        InvalidPopupAction = -1,
        Accept,
        Cancel,
        Yes,
        No,
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

    enum class UserType {
        Annonymous = -1,
        SuperUser,
        Inspector,
        PCA,
        Warehouseman,
        CooperativeRepresentative,

        UserTypeCount
    };
    Q_ENUM_NS(UserType)
    inline uint qHash(UserType key, uint seed) { return qH<UserType>(key, seed);}

    enum class PlaceType {
        Invalid = -1,
        Parcele,
        Section,
        CooperativeHQ,

        PlaceTypeCount
    };
    Q_ENUM_NS(PlaceType)
    inline uint qHash(PlaceType key, uint seed) { return qH<PlaceType>(key, seed);}

    enum class PackageType {
        Unknown = -1,
        Harvest,
        Sac,
        Lot,

        PackageTypeCount
    };
    Q_ENUM_NS(PackageType)
    inline uint qHash(PackageType key, uint seed) { return qH<PackageType>(key, seed);}

    enum class SupplyChainAction {
        Unknown = -1,
        Harvest,
        GrainProcessing,
        SectionReception,
        Bagging,
        LotCreation,
        WarehouseTransport,
        ExportReception,

        SupplyChainActionCount
    };
    Q_ENUM_NS(SupplyChainAction)
    inline uint qHash(SupplyChainAction key, uint seed) { return qH<SupplyChainAction>(key, seed);}

    // REMEMBER TO REGISTER ENUM IN maincontroller.setupQmlContext TO MAKE IT VISIBLE FOR QML!
}


#endif // ENUMS_H
