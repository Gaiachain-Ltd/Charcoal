pragma Singleton

import QtQuick 2.12

import "../"

AbstractStyle {
    readonly property color menuSupplyChainColor: "#C0DA95"
    readonly property color menuTrackingColor: "#D9D9D9"
    readonly property color menuReplantationColor: "#F2CBA4"
    readonly property color submenuLoggingBeginningColor: "#F8F8F8"
    readonly property color submenuLoggingEndingColor: "#EBF1E6"
    readonly property color submenuLoggingColor: submenuLoggingBeginningColor
    readonly property color submenuCarbonizationBeginningColor: submenuLoggingEndingColor
    readonly property color submenuCarbonizationEndingColor: submenuLoggingBeginningColor
    readonly property color submenuCarbonizationColor: submenuLoggingEndingColor
    readonly property color submenuLoadingAndTransportColor: submenuLoggingBeginningColor
    readonly property color submenuReceptionColor: submenuLoggingEndingColor

    readonly property url submenuCarbonizationBeginningUrl: "qrc:/ui/submenuCarbonizationBeginning"
    readonly property url submenuCarbonizationEndingUrl: "qrc:/ui/submenuCarbonizationEnding"
    readonly property url submenuCarbonizationUrl: "qrc:/ui/submenuCarbonization"
    readonly property url submenuLoadingAndTransportUrl: "qrc:/ui/submenuLoadingAndTransport"
    readonly property url submenuLoggingBeginningUrl: "qrc:/ui/submenuLoggingBeginning"
    readonly property url submenuLoggingEndingUrl: "qrc:/ui/submenuLoggingEnding"
    readonly property url submenuLoggingUrl: "qrc:/ui/submenuLogging"
    readonly property url submenuReceptionUrl: "qrc:/ui/submenuReception"
    readonly property url menuReplantationUrl: "qrc:/ui/menuReplantation"
    readonly property url menuSupplyChainUrl: "qrc:/ui/menuSupplyChain"
    readonly property url menuTrackingUrl: "qrc:/ui/menuTracking"

    logoImgUrl: logoWhiteImgUrl
    loginBackgroundUrl: "qrc:/ui/login-background"
    downArrowImgUrl: "qrc:/ui/arrow"

    primaryColor: "#000000"
    buttonSecondaryColor: "#888888"
    delegateHighlightColor: "#EEF9FE"
    fontHighlightColor: "#2581EB"
    delegateHighlightColor2: "#EBF1E6"
    fontHighlightColor2: "#65CB00"
    delegateHighlightColor3: "#FFF3E8"
    fontHighlightColor3: "#F8932E"
    delegateHighlightColor4: "#FFE8FD"
    fontHighlightColor4: "#FF70F1"

    menuButtonHeight: 300

    pixelSize: 29

    headerCapitalization: Font.AllUppercase
}
