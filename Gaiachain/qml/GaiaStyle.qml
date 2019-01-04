pragma Singleton

import QtQuick 2.11

QtObject {
    // Colors
    readonly property color textPrimaryColor: "black"
    readonly property color textSecondaryColor: "white"
    readonly property color textGreenColor: "green"
    readonly property color buttonGreenColor: "#96c354"

    readonly property real listEventsDelegateRelativeHeight: 390
    readonly property color listEventsDelegateBottomLineColor: greyButtonColor
    readonly property color greyButtonColor: "#ffcccccc"

    // mrgins
    readonly property int bigMargin: 45
    readonly property int normalMargin: 30
    readonly property int smallMargin: 15


    readonly property color buttonBackColor: "#FF5AB400"

    // Text
    readonly property real pixelSize: 40
    readonly property real bigPixelSize: 60

    // Components sizes
    readonly property int footerHeight: 150

    readonly property int headerHeight: 180
    readonly property int headerArrowWidth: 20
    readonly property int headerButtonsPadding: 20

    // RegisterBar
    readonly property color registerBarBackgroundColor: "black"

    // urls
    readonly property url blackAddImgUrl: "qrc:/ui/add_black"
    readonly property url greenAddImgUrl: "qrc:/ui/add_green"
    readonly property url rightArrowImgUrl: "qrc:/ui/right_arrow"
    readonly property url backImgUrl: "qrc:/ui/back"
    readonly property url backBlackImgUrl: "qrc:/ui/back_black"
    readonly property url calendarImgUrl: "qrc:/ui/calendar"

    readonly property url cancelImgUrl: "qrc:/ui/cancel"
    readonly property url detailsImgUrl: "qrc:/ui/details"
    readonly property url detailsGreenImgUrl: "qrc:/ui/details_green"
    readonly property url editlImgUrl: "qrc:/ui/edit"
    readonly property url exportImgUrl: "qrc:/ui/export"
    readonly property url logoImgUrl: "qrc:/ui/gaia_logo"

    readonly property url gaiaBlackImgUrl: "qrc:/ui/gaia_black"
    readonly property url gaiaColorImgUrl: "qrc:/ui/gaia_color"
    readonly property url homeImgUrl: "qrc:/ui/home"
    readonly property url keyboardImgUrl: "qrc:/ui/keyboard"
    readonly property url keyImgUrl: "qrc:/ui/key"

    readonly property url listImgUrl: "qrc:/ui/list"
    readonly property url loginImgUrl: "qrc:/ui/login"
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

    // button
    readonly property int buttonHeight: 90

    // Other
    readonly property int animationDuration: 300
    readonly property int animationEasing: Easing.Linear
}
