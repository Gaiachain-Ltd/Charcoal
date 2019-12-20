pragma Singleton

import QtQuick 2.12

QtObject {
    readonly property real logoHeight: 170

    readonly property int keyboardAnimationDuration: 200

    // Colors
    readonly property color primaryColor: "#65cb00"

    readonly property color blank: "transparent"

    readonly property color backgroundColor: "white"
    readonly property color backgroundSecondaryColor: "#f6f6f6"
    readonly property color backgroundDisabledColor: "#f2f2f2"
    readonly property color headerBackgroundColor: "#667176"
    readonly property color footerBackgroundColor: "#e8eaec"

    readonly property color textPrimaryColor: "black"
    readonly property color textSecondaryColor: "white"
    readonly property color textDisabledColor: "#a4a4a4"
    readonly property color textReadonlyColor: "#868686"
    readonly property color textLinkColor: primaryColor
    readonly property color notRequiredTextInputColor: "#8c8c8c"

    readonly property color buttonPrimaryColor: primaryColor
    readonly property color buttonSecondaryColor: "white"

    readonly property color buttonPopupAcceptColor: primaryColor
    readonly property color buttonPopupAcceptSecondaryColor: "black"
    readonly property color buttonPopupRejectColor: "#bebebe"

    readonly property color buttonMenuColor: "#f6f6f6"
    readonly property color buttonBorderColor: "#d8d8d8"
    readonly property color inputBorderColor: "#d6d6d6"

    readonly property color expandButtonSubmenuColor: "#e6e6e6"

    readonly property color separatorColor: "#d6d6d6"

    readonly property color disabledColorTint: "#C0a4a4a4"
    readonly property color backgroundShadowColor: "#99000000"
    readonly property color backgroundShadowMediumColor: "#99595959"
    readonly property color backgroundDarkShadowColor: "#CC000000"

    readonly property color delegateHighlightColor: "#f2f2f2"

    readonly property color unknownColor: "#8f8f8f"
    readonly property color okColor: "#65cb00"
    readonly property color warningColor: "#de8c00"
    readonly property color errorColor: "#cb0000"

    readonly property color harvestColor: "#2581eb"
    readonly property color sacColor: "#65cb00"
    readonly property color lotColor: "#ff70f1"

    readonly property color controlGradientStartColor: "#eff6f0"
    readonly property color controlGradientStopColor: "#ffffff"

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
    readonly property int listViewDelegateDefaultHeight: 120
    readonly property int separatorHeight: 1

    // urls
    readonly property url dummyTestIconUrl: "qrc:/ui/dummy/testIcon"

    readonly property url logoBlackImgUrl: "qrc:/ui/logo-black"
    readonly property url logoWhiteImgUrl: "qrc:/ui/logo-white"
    readonly property url logoImgUrl: logoBlackImgUrl
    readonly property url logoIconImgUrl: "qrc:/ui/logo-icon"

    readonly property url backImgUrl: "qrc:/ui/back"
    readonly property url closeImgUrl: "qrc:/ui/close"
    readonly property url closeBlackImgUrl: "qrc:/ui/close-black"
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

    readonly property url searchImgUrl: "qrc:/ui/search"
    readonly property url calendarBlackImgUrl: "qrc:/ui/calendarBlack"

    readonly property url returnCurrentMonthImgUrl: "qrc:/ui/returnCurrentMonth"

    readonly property url supplyChainSubmenuHarvestImgUrl: "qrc:/ui/submenuHarvest"
    readonly property url supplyChainSubmenuGrainProcessingImgUrl: "qrc:/ui/submenuGrainProcessing"
    readonly property url supplyChainSubmenuBaggingImgUrl: "qrc:/ui/submenuBagging"
    readonly property url supplyChainSubmenuLotCreationImgUrl: "qrc:/ui/submenuLotCreation"

    readonly property url supplyChainSubmenuTransportImgUrl: "qrc:/ui/submenuTransport"
    readonly property url supplyChainSubmenuReceptionImgUrl: "qrc:/ui/submenuReception"

    readonly property url leftBlackArrowImgUrl: "qrc:/ui/leftBlackArrow"
    readonly property url rightBlackArrowImgUrl: "qrc:/ui/rightBlackArrow"
    readonly property url upArrowImgUrl: "qrc:/ui/arrowUp"
    readonly property url downArrowImgUrl: "qrc:/ui/arrowDown"
    readonly property url rightArrowImgUrl: "qrc:/ui/arrowRight"

    readonly property url expandUpImgUrl: "qrc:/ui/expandUp"
    readonly property url expandDownImgUrl: "qrc:/ui/expandDown"
    readonly property url clearImgUrl: "qrc:/ui/clear"

    readonly property url plusImgUrl: "qrc:/ui/plus"
    readonly property url qrImgUrl: "qrc:/ui/qr"
    readonly property url threeDotsImgUrl: "qrc:/ui/threeDots"
    readonly property url deleteImgUrl: "qrc:/ui/delete"

    readonly property url checkImgUrl: "qrc:/ui/check"
    readonly property url frameImgUrl: "qrc:/ui/frame"

    readonly property url typeManuallyImgUrl: "qrc:/ui/typeManually"
    readonly property url enterImgUrl: "qrc:/ui/enter"

    readonly property url warningImgUrl: "qrc:/ui/warning"

    readonly property url gpsOkImgUrl: "qrc:/ui/gpsOk"
    readonly property url gpsFailedImgUrl: "qrc:/ui/gpsFailed"
    readonly property url gpsImgUrl: "qrc:/ui/gps"

    // control
    readonly property int controlDefaultBorderWidth: 1
    readonly property int controlBoldBorderWidth: 2
    readonly property int controlBoldestBorderWidth: 3

    // input
    readonly property int inputHeight: 90

    // button
    readonly property int buttonHeight: 90
    readonly property int buttonRadius: smallMargin
    readonly property int buttonSmallRadius: 10
    readonly property int buttonImageTinyHeight: 30
    readonly property int buttonImageSmallHeight: 55
    readonly property int buttonImageHeight: 95
    readonly property int buttonImageMiddleBigHeight: 115
    readonly property int buttonImageBigHeight: 125
    readonly property int buttonImageMargin: 20
    readonly property int buttonNumberRadius: middleMargin

    // Other
    readonly property int animationDuration: 300
    readonly property int animationEasing: Easing.Linear
    readonly property int requestOverlayInterval: 2000
    readonly property int commentPopupOpenedDefaultInterval: 2000
    readonly property int notificationPopupOpenedDefaultInterval: 2600
    readonly property int notificationPopupOpenedLongInterval: 5600
    readonly property int notificationPopupIconHeight: 60
    readonly property int transitionMoveYRange: 100
    readonly property int genericTextAreaHeight: 280

    // logical
    readonly property int visible: 1
    readonly property int hidden: 0

    // none
    readonly property int none: 0

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
    readonly property int calendarDotSize: 10
    readonly property int calendarCurrentDayBorderWidth: 3
    readonly property int calendarNamePixelSize: tinyPixelSize
    readonly property int calendarNumberPixelSize: smallPixelSize
    readonly property int calendarDayMinHeight: calendarNumberPixelSize + calendarDotSize + 2 * smallMargin
    readonly property int calendarDayChangeDuration: 1000
    readonly property int calendarMonthMinWidth: 150
    readonly property int calendarMonthMinHeight: 120
    readonly property int calendarYearStartYear: 2018
}
