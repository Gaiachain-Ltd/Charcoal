import QtQuick 2.12

QtObject {
    property real logoHeight: 170

    property int keyboardAnimationDuration: 200

    // Colors
    property color primaryColor: "#65cb00"

    property color blank: "transparent"

    property color backgroundColor: "white"
    property color backgroundSecondaryColor: "#f6f6f6"
    property color backgroundDisabledColor: "#f2f2f2"
    property color headerBackgroundColor: "#667176"
    property color footerBackgroundColor: "#e8eaec"

    property color textPrimaryColor: "black"
    property color textSecondaryColor: "white"
    property color textDisabledColor: "#a4a4a4"
    property color textReadonlyColor: "#868686"
    property color textLinkColor: primaryColor
    property color notRequiredTextInputColor: "#8c8c8c"
    property color textTabColor: "#7E7E7E"
    property color textCalendarDayRowColor: textPrimaryColor

    property color buttonPrimaryColor: primaryColor
    property color buttonSecondaryColor: "white"

    property color buttonPopupAcceptColor: primaryColor
    property color buttonPopupAcceptSecondaryColor: "black"
    property color buttonPopupRejectColor: "#bebebe"

    property color buttonMenuColor: "#f6f6f6"
    property color buttonBorderColor: "#d8d8d8"
    property color inputBorderColor: "#d6d6d6"

    property color expandButtonSubmenuColor: "#e6e6e6"

    property color separatorColor: "#d6d6d6"

    property color disabledColorTint: "#C0a4a4a4"
    property color backgroundShadowColor: "#99000000"
    property color backgroundShadowMediumColor: "#99595959"
    property color backgroundDarkShadowColor: "#CC000000"

    property color delegateHighlightColor: "#f2f2f2"
    property color fontHighlightColor: "#f2f2f2"

    property color delegateHighlightColor2: "#f6f6f6"
    property color fontHighlightColor2: "#f6f6f6"

    property color delegateHighlightColor3: "#fafafa"
    property color fontHighlightColor3: "#fafafa"

    property color delegateHighlightColor4: "#fdfdfd"
    property color fontHighlightColor4: "#fdfdfd"

    property color unknownColor: "#8f8f8f"
    property color okColor: "#65cb00"
    property color warningColor: "#de8c00"
    property color errorColor: "#cb0000"

    property color harvestColor: "#2581eb"
    property color sacColor: "#65cb00"
    property color lotColor: "#ff70f1"

    property color controlGradientStartColor: "#eff6f0"
    property color controlGradientStopColor: "#ffffff"

    // Margins
    property int hugeMargin: 40
    property int bigMargin: 35
    property int middleBigMargin: 30
    property int middleMargin: 25
    property int middleSmallMargin: 20
    property int smallMargin: 15
    property int tinyMargin: 10
    property int microMargin: 5
    property int picoMargin: 2

    // image
    property int imageSize: 30

    // Text
    property real tinyPixelSize: pixelSize * 0.7
    property real smallPixelSize: pixelSize * 0.8
    property real pixelSize: 40
    property real bigPixelSize: pixelSize * 1.5
    property real titlePixelSize: pixelSize * 1.2
    property real subtitlePixelSize: pixelSize * 1.1
    property real inputPixelSize: pixelSize * 0.9
    property real popupPixelSize: pixelSize * 1.1
    property real buttonPixelSize: pixelSize * 1.1

    // font
    property real buttonLetterSpacing: 10

    property string primaryFontFamily: "PublicSans"

    property var publicSansRegularFont: FontLoader {
        source: "qrc:/font/fonts/PublicSans/PublicSans-Regular.otf"
    }
    property var publicSansSemiBoldFont: FontLoader {
        source: "qrc:/font/fonts/PublicSans/PublicSans-SemiBold.otf"
    }
    property var publicSansBoldFont: FontLoader {
        source: "qrc:/font/fonts/PublicSans/PublicSans-Bold.otf"
    }

    property int headerCapitalization: Font.MixedCase

    // Components sizes
    property int headerHeight: 200
    property int footerHeight: 200
    property int listViewDelegateDefaultHeight: 120
    property int separatorHeight: 1

    // urls
    property url dummyTestIconUrl: "qrc:/ui/dummy/testIcon"

    property url logoBlackImgUrl: "qrc:/ui/logo-black"
    property url logoWhiteImgUrl: "qrc:/ui/logo-white"
    property url logoImgUrl: logoBlackImgUrl
    property url logoIconImgUrl: "qrc:/ui/logo-icon"

    property url backImgUrl: "qrc:/ui/back"
    property url closeImgUrl: "qrc:/ui/close"
    property url closeBlackImgUrl: "qrc:/ui/close-black"
    property url logoutImgUrl: "qrc:/ui/logout"
    property url userImgUrl: "qrc:/ui/user"

    property url emailImgUrl: "qrc:/ui/email"
    property url passwordImgUrl: "qrc:/ui/password"

    property url loadingLogoOneImgUrl: "qrc:/ui/loadingLogoOne"
    property url loadingLogoTwoImgUrl: "qrc:/ui/loadingLogoTwo"
    property url loadingLogoThreeImgUrl: "qrc:/ui/loadingLogoThree"

    property url refreshImgUrl: "qrc:/ui/refresh"
    property url spinnerImgUrl: "qrc:/ui/spinner"

    property url supplyChainButtonImgUrl: "qrc:/ui/menuSupplyChain"
    property url transactionsButtonImgUrl: "qrc:/ui/menuTransactions"
    property url calendarButtonImgUrl: "qrc:/ui/menuCalendar"

    property url searchImgUrl: "qrc:/ui/search"
    property url calendarBlackImgUrl: "qrc:/ui/calendarBlack"

    property url returnCurrentMonthImgUrl: "qrc:/ui/returnCurrentMonth"

    property url supplyChainSubmenuHarvestImgUrl: "qrc:/ui/submenuHarvest"
    property url supplyChainSubmenuGrainProcessingImgUrl: "qrc:/ui/submenuGrainProcessing"
    property url supplyChainSubmenuBaggingImgUrl: "qrc:/ui/submenuBagging"
    property url supplyChainSubmenuLotCreationImgUrl: "qrc:/ui/submenuLotCreation"

    property url supplyChainSubmenuTransportImgUrl: "qrc:/ui/submenuTransport"
    property url supplyChainSubmenuReceptionImgUrl: "qrc:/ui/submenuReception"

    property url leftBlackArrowImgUrl: "qrc:/ui/leftBlackArrow"
    property url rightBlackArrowImgUrl: "qrc:/ui/rightBlackArrow"
    property url upArrowImgUrl: "qrc:/ui/arrowUp"
    property url downArrowImgUrl: "qrc:/ui/arrowDown"
    property url rightArrowImgUrl: "qrc:/ui/arrowRight"

    property url expandUpImgUrl: "qrc:/ui/expandUp"
    property url expandDownImgUrl: "qrc:/ui/expandDown"
    property url clearImgUrl: "qrc:/ui/clear"

    property url plusImgUrl: "qrc:/ui/plus"
    property url qrImgUrl: "qrc:/ui/qr"
    property url threeDotsImgUrl: "qrc:/ui/threeDots"
    property url deleteImgUrl: "qrc:/ui/delete"

    property url checkImgUrl: "qrc:/ui/check"
    property url frameImgUrl: "qrc:/ui/frame"

    property url typeManuallyImgUrl: "qrc:/ui/typeManually"
    property url enterImgUrl: "qrc:/ui/enter"

    property url warningImgUrl: "qrc:/ui/warning"

    property url gpsOkImgUrl: "qrc:/ui/gpsOk"
    property url gpsFailedImgUrl: "qrc:/ui/gpsFailed"
    property url gpsImgUrl: "qrc:/ui/gps"

    property url helpButtonUrl: "qrc:/ui/helpButton"

    // control
    property int controlDefaultBorderWidth: 1
    property int controlBoldBorderWidth: 2
    property int controlBoldestBorderWidth: 3

    // input
    property int inputHeight: 90

    // button
    property int buttonHeight: 90
    property int buttonRadius: smallMargin
    property int buttonSmallRadius: 10
    property int buttonImageTinyHeight: 30
    property int buttonImageSmallHeight: 55
    property int buttonImageHeight: 95
    property int buttonImageMiddleBigHeight: 115
    property int buttonImageBigHeight: 125
    property int buttonImageMargin: 20
    property int buttonNumberRadius: middleMargin

    // Other
    property int animationDuration: 300
    property int animationEasing: Easing.Linear
    property int requestOverlayInterval: 2000
    property int commentPopupOpenedDefaultInterval: 2000
    property int notificationPopupOpenedDefaultInterval: 2600
    property int notificationPopupOpenedLongInterval: 5600
    property int qrOverlayInterval: notificationPopupOpenedLongInterval
    property int notificationPopupIconHeight: 60
    property int transitionMoveYRange: 100
    property int genericTextAreaHeight: 280

    // logical
    property int visible: 1
    property int hidden: 0

    // none
    property int none: 0

    // PAGES SPECIFIC
    // login loading
    property color loginLoadingTextColor: "#d6d6d6"
    property url loginBackgroundUrl: ""

    // main menu
    property int menuButtonHeight: 180
    property int submenuButtonHeight: 125
    property int menuButtonImageHeight: buttonImageBigHeight * 0.85
    property int submenuButtonImageHeight: buttonImageSmallHeight
    property real submenuButtonPixelSize: buttonPixelSize * 0.8
    property color menuButtonColor: "#f6f6f6"

    // calendar pages
    property color calendarArrowButtonColor: "#e8eaec"
    property int calendarDotSize: 10
    property int calendarCurrentDayBorderWidth: 3
    property int calendarNamePixelSize: tinyPixelSize
    property int calendarNumberPixelSize: smallPixelSize
    property int calendarDayMinHeight: calendarNumberPixelSize + calendarDotSize + 2 * smallMargin
    property int calendarDayChangeDuration: 1000
    property int calendarMonthMinWidth: 150
    property int calendarMonthMinHeight: 120
    property int calendarYearStartYear: 2018

    // help page
    property color helpPageBackgroundColor: "#96C354"
    property url helpPageImageUrl: "qrc:/ui/helpImage"
}
