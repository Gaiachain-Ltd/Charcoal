pragma Singleton

import QtQuick 2.11

import com.gaiachain.enums 1.0

QtObject {
    readonly property bool loginByCombobox: utility.isLoginComboboxVisible()

    readonly property real logoSize: 0.55

    // Colors
    readonly property color primaryColor: "#65cb00"

    readonly property color backgroundColor: "white"
    readonly property color backgroundSecondaryColor: "#F6F6F6"
    readonly property color headerBackgroundColor: "#667176"
    readonly property color footerBackgroundColor: "#e8eaec"

    readonly property color textPrimaryColor: "black"
    readonly property color textSecondaryColor: "white"
    readonly property color textDisabledColor: "#a4a4a4"
    readonly property color textLinkColor: primaryColor

    readonly property color buttonPrimaryColor: primaryColor
    readonly property color buttonSecondaryColor: "white"

    readonly property color buttonPopupAcceptColor: primaryColor
    readonly property color buttonPopupAcceptSecondaryColor: "black"
    readonly property color buttonPopupRejectColor: "#bebebe"

    readonly property color buttonMenuColor: "#f6f6f6"

    readonly property color disabledColorTint: "#C0a4a4a4"
    readonly property color backgroundShadowColor: "#99000000"

    readonly property color okColor: "#65cb00"
    readonly property color warningColor: "#de8c00"
    readonly property color errorColor: "#cb0000"

    readonly property color recolteColor: "#2581eb"
    readonly property color sacColor: "#65cb00"
    readonly property color lotColor: "#ff70f1"

    // Margins
    readonly property int hugeMargin: 45
    readonly property int bigMargin: 40
    readonly property int normalMargin: 30
    readonly property int smallMargin: 15
    readonly property int tinyMargin: 10

    // image
    readonly property int imageSize: 30

    // Text
    readonly property real smallPixelSize: pixelSize * 0.8
    readonly property real pixelSize: 45
    readonly property real bigPixelSize: pixelSize * 1.5
    readonly property real titlePixelSize: pixelSize * 1.2
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
    readonly property int headerHeight: 210
    readonly property int footerHeight: 210

    // urls
    readonly property url logoImgUrl: "qrc:/ui/logo"
    readonly property url logoIconImgUrl: "qrc:/ui/logo_icon"

    readonly property url backImgUrl: "qrc:/ui/back"
    readonly property url logoutImgUrl: "qrc:/ui/logout"
    readonly property url userImgUrl: "qrc:/ui/user"

    readonly property url emailImgUrl: "qrc:/ui/email"
    readonly property url passwordImgUrl: "qrc:/ui/password"

    readonly property url loadingLogoOneImgUrl: "qrc:/ui/loadingLogoOne"
    readonly property url loadingLogoTwoImgUrl: "qrc:/ui/loadingLogoTwo"
    readonly property url loadingLogoThreeImgUrl: "qrc:/ui/loadingLogoThree"

    readonly property url refreshImgUrl: "qrc:/ui/refresh"
    readonly property url spinnerImgUrl: "qrc:/ui/spinner"

    // input
    readonly property int inputHeight: buttonHeight
    readonly property color inputBorderColor: "#e0e0e0"

    // button
    readonly property int buttonHeight: 90
    readonly property int smallButtonHeight: 40
    readonly property int buttonRadius: 20
    readonly property int bigButtonRadius: 28
    readonly property int defaultPadding: smallMargin * 1.5

    // Other
    readonly property int animationDuration: 300
    readonly property int animationEasing: Easing.Linear
    readonly property int requestOverlayInterval: 2000
}
