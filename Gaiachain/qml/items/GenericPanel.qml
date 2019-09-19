import QtQuick 2.11

import QtQuick.Controls 2.4

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

Page {
    property int page: Enums.Page.InvalidPage
    property bool isPopup: false

    property var attachedStackView: StackView.view

    function closeEventHandler()
    {
        return false
    }

    palette {
        alternateBase: Style.backgroundSecondaryColor
        base: Style.backgroundColor
        brightText: Style.textSecondaryColor
        button: Style.buttonPrimaryColor
        buttonText: Style.textSecondaryColor
        dark: Style.textPrimaryColor
        highlight: Style.primaryColor
        highlightedText: Style.textSecondaryColor
        light: Style.backgroundColor
        link: Style.textLinkColor
        linkVisited: Style.textDisabledColor
        mid: Style.footerBackgroundColor
        midlight: Style.backgroundSecondaryColor
        shadow: Style.backgroundShadowColor
        text: Style.textPrimaryColor
        toolTipBase: Style.backgroundSecondaryColor
        toolTipText: Style.textPrimaryColor
        window: Style.backgroundColor
        windowText: Style.textPrimaryColor
    }
}
