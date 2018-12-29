pragma Singleton

import QtQuick 2.11

QtObject {
    // Colors
    readonly property color textPrimaryColor: "black"
    readonly property color textSecondaryColor: "white"
    readonly property color textGreenColor: "green"

    // Text
    readonly property real pixelSize: 40

    // Components sizes
    readonly property real footerHeight: 150
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

    // Other
    readonly property int animationDuration: 300
    readonly property int animationEasing: Easing.Linear
}
