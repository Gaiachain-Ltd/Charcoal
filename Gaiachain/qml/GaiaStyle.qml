pragma Singleton

import QtQuick 2.11

import com.gaiachain.enums 1.0

QtObject {
    readonly property real logoHeight: 170

    // Colors
    readonly property color primaryColor: "#65cb00"

    readonly property color backgroundColor: "white"
    readonly property color backgroundSecondaryColor: "#f6f6f6"
    readonly property color headerBackgroundColor: "#667176"
    readonly property color footerBackgroundColor: "#e8eaec"

    readonly property color textPrimaryColor: "black"
    readonly property color textSecondaryColor: "white"
    readonly property color textDisabledColor: "#a4a4a4"
    readonly property color textLinkColor: primaryColor

    readonly property color buttonPrimaryColor: primaryColor
    readonly property color buttonSecondaryColor: "white"

    readonly property color buttonPopupAcceptColor: primaryColor
    readonly property color buttonPopupAcceptSecondaryColor: "black"
    readonly property color buttonPopupRejectColor: "#bebebe"

    readonly property color buttonMenuColor: "#f6f6f6"
    readonly property color buttonBorderColor: "#d8d8d8"
    readonly property color inputBorderColor: "#d6d6d6"

    readonly property color separatorColor: "#d6d6d6"

    readonly property color disabledColorTint: "#C0a4a4a4"
    readonly property color backgroundShadowColor: "#99000000"

    readonly property color unknownColor: "#8f8f8f"
    readonly property color okColor: "#65cb00"
    readonly property color warningColor: "#de8c00"
    readonly property color errorColor: "#cb0000"

    readonly property color harvestColor: "#2581eb"
    readonly property color sacColor: "#65cb00"
    readonly property color lotColor: "#ff70f1"

    // Margins
    readonly property int hugeMargin: 40
    readonly property int bigMargin: 35
    readonly property int middleBigMargin: 30
    readonly property int middleMargin: 25
    readonly property int middleSmallMargin: 20
    readonly property int smallMargin: 15
    readonly property int tinyMargin: 10

    // image
    readonly property int imageSize: 30

    // Text
    readonly property real tinyPixelSize: pixelSize * 0.7
    readonly property real smallPixelSize: pixelSize * 0.8
    readonly property real pixelSize: 40
    readonly property real bigPixelSize: pixelSize * 1.5
    readonly property real titlePixelSize: pixelSize * 1.2
    readonly property real subtitlePixelSize: pixelSize * 1.1
    readonly property real inputPixelSize: pixelSize * 0.9
    readonly property real popupPixelSize: pixelSize * 1.1
    readonly property real buttonPixelSize: pixelSize * 1.1

    // font
    readonly property real buttonLetterSpacing: 10

    readonly property string primaryFontFamily: "PublicSans"

    readonly property var publicSansRegularFont: FontLoader {
        source: "qrc:/font/fonts/PublicSans/PublicSans-Regular.otf"
    }
    readonly property var publicSansSemiBoldFont: FontLoader {
        source: "qrc:/font/fonts/PublicSans/PublicSans-SemiBold.otf"
    }
    readonly property var publicSansBoldFont: FontLoader {
        source: "qrc:/font/fonts/PublicSans/PublicSans-Bold.otf"
    }

    // Components sizes
    readonly property int headerHeight: 200
    readonly property int footerHeight: 200
    readonly property int separatorHeight: 1

    // urls
    readonly property url dummyTestIconUrl: "qrc:/ui/dummy/testIcon"

    readonly property url logoBlackImgUrl: "qrc:/ui/logo-black"
    readonly property url logoWhiteImgUrl: "qrc:/ui/logo-white"
    readonly property url logoImgUrl: logoBlackImgUrl
    readonly property url logoIconImgUrl: "qrc:/ui/logo-icon"

    readonly property url backImgUrl: "qrc:/ui/back"
    readonly property url closeImgUrl: "qrc:/ui/close"
    readonly property url logoutImgUrl: "qrc:/ui/logout"
    readonly property url userImgUrl: "qrc:/ui/user"

    readonly property url emailImgUrl: "qrc:/ui/email"
    readonly property url passwordImgUrl: "qrc:/ui/password"

    readonly property url loadingLogoOneImgUrl: "qrc:/ui/loadingLogoOne"
    readonly property url loadingLogoTwoImgUrl: "qrc:/ui/loadingLogoTwo"
    readonly property url loadingLogoThreeImgUrl: "qrc:/ui/loadingLogoThree"

    readonly property url refreshImgUrl: "qrc:/ui/refresh"
    readonly property url spinnerImgUrl: "qrc:/ui/spinner"

    readonly property url supplyChainButtonImgUrl: "qrc:/ui/menuSupplyChain"
    readonly property url transactionsButtonImgUrl: "qrc:/ui/menuTransactions"
    readonly property url calendarButtonImgUrl: "qrc:/ui/menuCalendar"
    readonly property url trackingButtonImgUrl: "qrc:/ui/menuTracking"

    readonly property url supplyChainSubmenuHarvestImgUrl: "qrc:/ui/submenuHarvest"
    readonly property url supplyChainSubmenuFermentationImgUrl: "qrc:/ui/submenuFermentation"
    readonly property url supplyChainSubmenuBaggingImgUrl: "qrc:/ui/submenuBagging"
    readonly property url supplyChainSubmenuStorageArrivalImgUrl: "qrc:/ui/submenuStorageArrival"
    readonly property url supplyChainSubmenuStorageLotImgUrl: "qrc:/ui/submenuStorageLot"
    readonly property url supplyChainSubmenuTransportImgUrl: "qrc:/ui/submenuTransport"
    readonly property url supplyChainSubmenuReceptionImgUrl: "qrc:/ui/submenuReception"

    readonly property url leftArrowImgUrl: "qrc:/ui/leftArrow"
    readonly property url rightArrowImgUrl: "qrc:/ui/rightArrow"

    // input
    readonly property int inputHeight: 90

    // button
    readonly property int buttonRadius: smallMargin
    readonly property int buttonSmallRadius: 10
    readonly property int buttonImageSmallHeight: 55
    readonly property int buttonImageHeight: 95
    readonly property int buttonImageBigHeight: 125
    readonly property int buttonImageMargin: 20

    // Other
    readonly property int animationDuration: 300
    readonly property int animationEasing: Easing.Linear
    readonly property int requestOverlayInterval: 2000

    // PAGES SPECIFIC
    // login loading
    readonly property color loginLoadingTextColor: "#d6d6d6"

    // main menu
    readonly property int menuButtonHeight: 180
    readonly property int submenuButtonHeight: 125
    readonly property int menuButtonImageHeight: buttonImageBigHeight * 0.85
    readonly property int submenuButtonImageHeight: buttonImageSmallHeight
    readonly property real submenuButtonPixelSize: buttonPixelSize * 0.8
    readonly property color menuButtonColor: "#f6f6f6"

    // calendar pages
    readonly property color calendarArrowButtonColor: "#e8eaec"
    readonly property int calendarDotSize: 15
    readonly property int calendarCurrentDayBorderWidth: 3
    readonly property int calendarNamePixelSize: tinyPixelSize
    readonly property int calendarNumberPixelSize: smallPixelSize
    readonly property int calendarDayMinHeight: calendarNumberPixelSize + calendarDotSize + 2 * smallMargin
    readonly property int calendarDayChangeDuration: 1000
}
