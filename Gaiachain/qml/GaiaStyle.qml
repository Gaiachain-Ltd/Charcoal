pragma Singleton

import QtQuick 2.11

QtObject {
    // Colors
    readonly property color textPrimaryColor: "black"
    readonly property color textSecondaryColor: "white"
    readonly property color textGreenColor: "green"
    readonly property color buttonGreenColor: "#96c354"

    // Text
    readonly property real pixelSize: 40
    readonly property real bigPixelSize: 60

    // Components sizes
    readonly property real footerHeight: 180
    readonly property real headerHeight: 150

    // RegisterBar
    readonly property color registerBarBackgroundColor: "black"

    // urls
    readonly property url timberImgUrl: "qrc:/ui/timber"
    readonly property url charcoalImgUrl: "qrc:/ui/charcoal"
    readonly property url cocoaImgUrl: "qrc:/ui/cocoa"

    readonly property url calendarImgUrl: "qrc:/ui/calendar"
    readonly property url mapImgUrl: "qrc:/ui/map"

    readonly property url backImgUrl: "qrc:/ui/back"
    readonly property url homeImgUrl: "qrc:/ui/home"

    readonly property url registerImgUrl: "qrc:/ui/map"

    readonly property url buttonAddGreen: "qrc:/ui/add_green"
    readonly property url buttonAddBlack: "qrc:/ui/add_black"

    // button
    readonly property int buttonHeight: 120

    // Other
    readonly property int animationDuration: 300
    readonly property int animationEasing: Easing.Linear
}
