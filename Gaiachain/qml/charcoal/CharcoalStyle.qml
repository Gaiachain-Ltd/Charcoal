pragma Singleton

import QtQuick 2.12

import "../"

AbstractStyle {
    property color menuSupplyChainColor: "#C0DA95"
    property color menuTrackingColor: "#D9D9D9"
    property color menuReplantationColor: "#F2CBA4"

    logoImgUrl: logoWhiteImgUrl
    loginBackgroundUrl: "qrc:/ui/login-background"

    primaryColor: "#000000"
    buttonSecondaryColor: "#888888"

    pixelSize: 29
}
