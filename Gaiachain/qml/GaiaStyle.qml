pragma Singleton

import QtQuick 2.11

QtObject {
    // Colors
    readonly property color backgroundColor: "white"
    readonly property color textPrimaryColor: "black"
    readonly property color textSecondaryColor: "white"
    readonly property color textErrorColor: "red"
    readonly property color textGreyColor: "#999999"
    readonly property color textGreenColor: "#5AB400"
    readonly property color buttonGreenColor: "#96c354"
    readonly property color buttonBlackGreyColor: "#666666"
    readonly property color buttonGreyColor: "#cccccc"
    readonly property color disabledButtonGreyColor: "#E6E6E6"
    readonly property color backgroundShadowColor: "#99000000"

    readonly property color listEventsDelegateBottomLineColor: buttonGreyColor

    readonly property color pageBaseBackgroundColor: "white"
    readonly property color registerBarBackgroundColor: "black"

    // Margins
    readonly property int bigMargin: 45
    readonly property int normalMargin: 30
    readonly property int smallMargin: 15
    readonly property int tinyMargin: 10

    // image
    readonly property int imageSize: 30

    readonly property color buttonBackColor: textGreenColor

    // Text
    readonly property real smallPixelSize: 30
    readonly property real pixelSize: 40
    readonly property real bigPixelSize: 60
    readonly property real headerTitlePixelSize: pixelSize + 3
    readonly property real buttonPixelSize: pixelSize + 3

    // font
    readonly property string primaryFontFamily: "Lato"
    readonly property string secondaryFontFamily: "Montserrat"

    // Components sizes
    readonly property int footerHeight: 140
    readonly property int headerHeight: 175
    readonly property int headerArrowWidth: 20
    readonly property int headerArrowHeight: headerArrowWidth * 1.5
    readonly property int calendarSmallDotSize: 8
    readonly property int calendarBigDotSize: calendarSmallDotSize * 2

    // urls
    readonly property url blackAddImgUrl: "qrc:/ui/add_black"
    readonly property url greenAddImgUrl: "qrc:/ui/add_green"
    readonly property url rightArrowImgUrl: "qrc:/ui/right_arrow"
    readonly property url rightArrowLightImgUrl: "qrc:/ui/right_arrow_light"
    readonly property url backImgUrl: "qrc:/ui/back"
    readonly property url backBlackImgUrl: "qrc:/ui/back_black"
    readonly property url calendarImgUrl: "qrc:/ui/calendar"

    readonly property url cancelImgUrl: "qrc:/ui/cancel"
    readonly property url relaodImgUrl: "qrc:/ui/reload"
    readonly property url detailsImgUrl: "qrc:/ui/details"
    readonly property url detailsGreenImgUrl: "qrc:/ui/details_green"
    readonly property url editImgUrl: "qrc:/ui/edit"
    readonly property url editGreenImgUrl: "qrc:/ui/edit_green"
    readonly property url exitToLoginImgUrl: "qrc:/ui/exit_to_login"
    readonly property url exportImgUrl: "qrc:/ui/export"
    readonly property url logoImgUrl: "qrc:/ui/gaia_logo"

    readonly property url gaiaBlackImgUrl: "qrc:/ui/gaia_black"
    readonly property url gaiaColorImgUrl: "qrc:/ui/gaia_color"
    readonly property url homeImgUrl: "qrc:/ui/home"
    readonly property url keyboardImgUrl: "qrc:/ui/keyboard"
    readonly property url keyImgUrl: "qrc:/ui/key"
    readonly property url emailImgUrl: "qrc:/ui/email"

    readonly property url listImgUrl: "qrc:/ui/list"
    readonly property url loginImgUrl: "qrc:/ui/login"
    readonly property url logoutImgUrl: "qrc:/ui/logout"
    readonly property url logParkImgUrl: "qrc:/ui/log_park"
    readonly property url miniCalendarImgUrl: "qrc:/ui/mini_calendar"
    readonly property url miniCalendarGreenImgUrl: "qrc:/ui/mini_calendar_green"
    readonly property url miniEditImgUrl: "qrc:/ui/mini_edit"

    readonly property url miniListImgUrl: "qrc:/ui/mini_list"
    readonly property url miniListGreenImgUrl: "qrc:/ui/mini_list_green"
    readonly property url okImgUrl: "qrc:/ui/ok"
    readonly property url qrCodeImgUrl: "qrc:/ui/qr_code"
    readonly property url qrCodeGreenImgUrl: "qrc:/ui/qr_code_green"
    readonly property url sawmillImgUrl: "qrc:/ui/sawmill"
    readonly property url skipArrowImgUrl: "qrc:/ui/skip_arrow"

    readonly property url timberImgUrl: "qrc:/ui/timber"
    readonly property url miniTimberImgUrl: "qrc:/ui/mini_timber"
    readonly property url miniTimberGreenImgUrl: "qrc:/ui/mini_timber_green"
    readonly property url timelineImgUrl: "qrc:/ui/timeline"
    readonly property url timelineGreenImgUrl: "qrc:/ui/timeline_green"

    readonly property url buttonAddGreen: "qrc:/ui/add_green"
    readonly property url buttonAddBlack: "qrc:/ui/add_black"

    readonly property url spinnerImgUrl: "qrc:/ui/spinner"

    // camera
    readonly property color normalBorderColor: "white"
    readonly property color errorBorderColor: "red"
    readonly property color finishedBorderColor: buttonGreenColor

    // input
    readonly property int inputHeight: buttonHeight

    // button
    readonly property int buttonHeight: 90
    readonly property int smallButtonHeight: 40
    readonly property int buttonRadius: 20
    readonly property int bigButtonRadius: 28
    readonly property int defaultPadding: smallMargin * 1.5

    // Other
    readonly property int animationDuration: 300
    readonly property int animationEasing: Easing.Linear
    readonly property int headerMaximumButtonCount: 5

    readonly property var latoRegularFont: FontLoader {
        source: "qrc:/font/fonts/Lato-Regular.ttf"
    }
    readonly property var latoBoldFont: FontLoader {
        source: "qrc:/font/fonts/Lato-Bold.ttf"
    }
    readonly property var latoMontserratFont: FontLoader {
        source: "qrc:/font/fonts/Montserrat-Medium.ttf"
    }
}
