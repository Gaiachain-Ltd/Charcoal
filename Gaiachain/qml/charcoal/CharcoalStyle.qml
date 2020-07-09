pragma Singleton

import QtQuick 2.12

import "../"

AbstractStyle {
    readonly property color photoGalleryBackgroundColor: "black"

    readonly property color menuSupplyChainColor: "#C0DA95"
    readonly property color menuTrackingColor: "#D9D9D9"
    readonly property color menuReplantationColor: "#F2CBA4"
    readonly property color submenuLoggingBeginningColor: "#F8F8F8"
    readonly property color submenuLoggingEndingColor: delegateHighlightColor2
    readonly property color submenuLoggingColor: submenuLoggingBeginningColor
    readonly property color submenuCarbonizationBeginningColor: submenuLoggingEndingColor
    readonly property color submenuCarbonizationEndingColor: submenuLoggingBeginningColor
    readonly property color submenuCarbonizationColor: submenuLoggingEndingColor
    readonly property color submenuLoadingAndTransportColor: submenuLoggingBeginningColor
    readonly property color submenuReceptionColor: submenuLoggingEndingColor

    readonly property color calendarGridColor: delegateHighlightColor2
    readonly property color calendarCurrentGridColor: "#65CB00"
    readonly property color calendarBlockedColor: "#DFE8D7"
    readonly property color calendarBlockedFontColor: "#B1BFA3"
    readonly property color calendarNotCurrentColor: "#BEBEBE"

    readonly property color notificationWarningColor: warningColor

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
    readonly property url noPhotoUrl: "qrc:/ui/noPhoto"
    readonly property url skipUrl: "qrc:/ui/skip"
    readonly property url uploadOkUrl: "qrc:/ui/uploadOk"
    readonly property url warningUrl: "qrc:/ui/warning"
    readonly property url checkBlackUrl: "qrc:/ui/checkBlack"
    readonly property url checkBlueUrl: "qrc:/ui/checkBlue"
    readonly property url checkGreenUrl: "qrc:/ui/checkGreen"
    readonly property url checkPinkUrl: "qrc:/ui/checkPink"
    readonly property url checkOrangeUrl: "qrc:/ui/checkOrange"

    readonly property url loupeUrl: "qrc:/ui/loupe"
    readonly property url bagUrl: "qrc:/ui/bag"
    readonly property url infoUrl: "qrc:/ui/info"
    readonly property url bagsLoadingUrl: "qrc:/ui/bagsLoading"
    readonly property url bagsReceptionUrl: "qrc:/ui/bagsReception"
    readonly property url iconPhotoCameraGreenUrl: "qrc:/ui/iconPhotoCameraGreen"
    readonly property url iconPhotoCameraUrl: "qrc:/ui/iconPhotoCamera"
    readonly property url truckUrl: "qrc:/ui/truck"

    readonly property url iconKeyboardUrl: "qrc:/ui/iconKeyboard"
    readonly property url iconListUrl: "qrc:/ui/iconList"
    readonly property url iconNoUrl: "qrc:/ui/iconNo"
    readonly property url iconScanAnotherBlackUrl: "qrc:/ui/iconScanAnotherBlack"
    readonly property url iconScanAnotherGreenUrl: "qrc:/ui/iconScanAnotherGreen"
    readonly property url iconScanAnotherOrangeUrl: "qrc:/ui/iconScanAnotherOrange"
    readonly property url iconScanAnotherRedUrl: "qrc:/ui/iconScanAnotherRed"
    readonly property url iconYesUrl: "qrc:/ui/iconYes"
    readonly property url iconRetryScanBlackUrl: "qrc:/ui/iconRetryScanBlack"
    readonly property url iconRetryScanGreenUrl: "qrc:/ui/iconRetryScanGreen"
    readonly property url iconRetryScanOrangeUrl: "qrc:/ui/iconRetryScanOrange"
    readonly property url iconRetryScanRedUrl: "qrc:/ui/iconRetryScanRed"
    readonly property url iconDeleteAll: "qrc:/ui/iconDeleteAll"

    readonly property color statusGreen: "#65CB00"
    readonly property color statusYellow: "#FFBB00"
    readonly property color statusRed: "#E40000"
    readonly property color skipColor: "#474E51"

    readonly property url loginBackgroundOriginalUrl: "qrc:/ui/loginBackgroundOriginal"

    logoImgUrl: logoWhiteImgUrl
    loginBackgroundUrl: "qrc:/ui/loginBackground"

    primaryColor: "#000000"
    buttonSecondaryColor: "#888888"
    delegateHighlightColor: "#EEF9FE"
    fontHighlightColor: harvestColor
    delegateHighlightColor2: "#EBF1E6"
    fontHighlightColor2: sacColor
    delegateHighlightColor3: "#FFF3E8"
    fontHighlightColor3: "#F8932E"
    delegateHighlightColor4: "#FFE8FD"
    fontHighlightColor4: lotColor

    buttonBorderColor: inputBorderColor

    calendarArrowButtonColor: "transparent"
    calendarNumberPixelSize: tinyPixelSize
    calendarDayMinHeight: calendarNumberPixelSize + (2 * picoMargin)

    menuButtonHeight: 340

    headerCapitalization: Font.AllUppercase
}
