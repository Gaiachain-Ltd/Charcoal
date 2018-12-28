pragma Singleton

import QtQuick 2.11

QtObject {
    readonly property color textPrimaryColor: "black"
    readonly property color textSecondaryColor: "white"

    // RegisterBar
    readonly property color registerBarBackgroundColor: "black"

    // urls
    readonly property url timberImgUrl: "qrc:/ui/timber"
    readonly property url charcoalImgUrl: "qrc:/ui/charcoal"
    readonly property url cocoaImgUrl: "qrc:/ui/cocoa"

    readonly property url backImgUrl: "qrc:/ui/back"
    readonly property url homeImgUrl: "qrc:/ui/home"

    readonly property url registerImgUrl: "qrc:/ui/map"
}
