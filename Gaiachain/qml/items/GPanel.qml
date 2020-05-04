import QtQuick 2.11

import QtQuick.Controls 2.4

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

Page
{
    property int page: Enums.Page.InvalidPage
    property bool isPopup: false

    property var attachedStackView: StackView.view

    function closeEventHandler()
    {
        return false
    }

    palette {
        alternateBase: GStyle.backgroundSecondaryColor
        base: GStyle.backgroundColor
        brightText: GStyle.textSecondaryColor
        button: GStyle.buttonPrimaryColor
        buttonText: GStyle.textSecondaryColor
        dark: GStyle.textPrimaryColor
        highlight: GStyle.primaryColor
        highlightedText: GStyle.textSecondaryColor
        light: GStyle.backgroundColor
        link: GStyle.textLinkColor
        linkVisited: GStyle.textDisabledColor
        mid: GStyle.footerBackgroundColor
        midlight: GStyle.backgroundSecondaryColor
        shadow: GStyle.backgroundShadowColor
        text: GStyle.textPrimaryColor
        toolTipBase: GStyle.backgroundSecondaryColor
        toolTipText: GStyle.textPrimaryColor
        window: GStyle.backgroundColor
        windowText: GStyle.textPrimaryColor
    }

    locale: Qt.locale()
}
