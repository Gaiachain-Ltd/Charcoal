import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Window 2.2

import com.gaiachain.style 1.0

import "./items" as Items

ApplicationWindow
{
    id: mainWindow

    width: isDesktop ? 480 : Screen.desktopAvailableWidth
    height: isDesktop ? 854 : Screen.desktopAvailableHeight
    visible: true

    color: Style.backgroundColor

    function s(num) {
        return utility.scaleByDpi(num)
    }

    function sr(num) {
        return utility.scaleRoundByDpi(num)
    }


    MainStackView {
        id: mainStackView
        anchors.fill: parent
        anchors.bottomMargin:  Qt.inputMethod.keyboardRectangle.height > 0 ? Qt.inputMethod.keyboardRectangle.height - Math.round(s(Style.footerHeight)) : 0
    }

    onClosing: close.accepted = mainStackView.onClosingEvent()
}
