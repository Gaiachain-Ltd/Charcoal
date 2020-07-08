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

    enum class Page {
        InvalidPage = -1,
        CalendarEvents,
        CalendarYear,
        Calendar,
        Login,
        LoginLoading,
        MainMenu,
        QRScanner,
        Transactions,
        WaitingTransactions,
        PackageData,
        Help,
        SupplyChainSummary,
#ifdef COCOA
        Tracking,
        SupplyChainHarvest,
        SupplyChainGrainProcessing,
        SupplyChainSectionReception,
        SupplyChainBagging,
        SupplyChainAddHarvestId,
        SupplyChainLotCreation,
        SupplyChainWarehouseTransport,
        SupplyChainExportReception,
#elif CHARCOAL
        SupplyChainLoggingBeginning,
        SupplyChainLoggingEnding,
        SupplyChainCarbonizationBeginning,
        SupplyChainCarbonizationEnding,
        SupplyChainLoadingAndTransport,
        SupplyChainReception,
        SupplyChainFinalize,
        PageTracking,
        PageReplantation,
        UserInfo,
        TakeDocumentPictures,
        PhotoGallery,
#endif

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
        Comment,
        Text,
        About,
        NotificationWithLink,

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

    enum class Edge {
        Unknown = -1,
        LeftEdge,
        RightEdge
    };
    Q_ENUM_NS(Edge)
    inline uint qHash(Edge key, uint seed) { return qH<Edge>(key, seed);}

    enum class QRScanStatus {
        Unknown = -1,
        Scanning,
        Success,
        Failed
    };
    Q_ENUM_NS(QRScanStatus)
    inline uint qHash(QRScanStatus key, uint seed) { return qH<QRScanStatus>(key, seed);}

    enum class ConnectionState {
        Unknown = -1,
        ConnectionSuccessful,
        Connecting,
        ConnectionError,

        ConnectionStateCount
    };
    Q_ENUM_NS(ConnectionState)
    inline uint qHash(ConnectionState key, uint seed) { return qH<ConnectionState>(key, seed);}

    enum class AdditionalDataType {
        UnknownData = -1,
        ProducersData,
        ParcelsData,
        CompaniesData,
        DestinationsData
    };
    Q_ENUM_NS(AdditionalDataType)
    inline uint qHash(AdditionalDataType key, uint seed) { return qH<AdditionalDataType>(key, seed);}

    enum class UserType {
        Annonymous = -1,
#ifdef COCOA
        SuperUser,
        Inspector,
        PCA,
        Warehouseman,
        CooperativeRepresentative,
#elif CHARCOAL
        SuperUser,
        Director = SuperUser,
        Logger,
        Carbonizer,
#endif

        UserTypeCount
    };
    Q_ENUM_NS(UserType)
    inline uint qHash(UserType key, uint seed) { return qH<UserType>(key, seed);}

    enum class CompanyType {
        Unknown = -1,
        Cooperative,
        Buyer,
        Transporter,

        CompanyTypeCount
    };
    Q_ENUM_NS(CompanyType)
    inline uint qHash(CompanyType key, uint seed) { return qH<CompanyType>(key, seed);}

    enum class PackageType {
        Unknown = -1,
#ifdef COCOA
        Harvest,
        Sac,
        Lot,
#elif CHARCOAL
        Plot = 0x01,
        Harvest = 0x02,
        Transport = 0x04,
#endif

        PackageTypeCount
    };
    Q_DECLARE_FLAGS(PackageTypes, PackageType)
    Q_FLAG_NS(PackageTypes)
    Q_ENUM_NS(PackageType)
    inline uint qHash(PackageType key, uint seed) { return qH<PackageType>(key, seed);}

    enum class SupplyChainAction {
        Unknown = -1,
#ifdef COCOA
        Harvest,
        GrainProcessing,
        SectionReception,
        Bagging,
        LotCreation,
        WarehouseTransport,
        ExportReception,
#elif CHARCOAL
        LoggingBeginning,
        LoggingEnding,
        CarbonizationBeginning,
        CarbonizationEnding,
        LoadingAndTransport,
        Reception,
        Tracking,
        Replantation,
#endif

        SupplyChainActionCount
    };
    Q_ENUM_NS(SupplyChainAction)
    inline uint qHash(SupplyChainAction key, uint seed) { return qH<SupplyChainAction>(key, seed);}

    enum class DelegateType {
        Standard,
        Row,
        Column,
        ColumnStack,
        Collapsible,
        BeginEndRow
    };
    Q_ENUM_NS(DelegateType)
    inline uint qHash(DelegateType key, uint seed) { return qH<DelegateType>(key, seed);}

    // REMEMBER TO REGISTER ENUM IN maincontroller.setupQmlContext TO MAKE IT VISIBLE FOR QML!
}

Q_DECLARE_OPERATORS_FOR_FLAGS(Enums::PackageTypes)

#endif // ENUMS_H
