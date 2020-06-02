import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Window 2.2

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0
import com.gaiachain.static 1.0
import com.gaiachain.helpers 1.0

import "items" as Items

ApplicationWindow
{
    id: mainWindow

    width: mainController.isDesktop ? 480 : Screen.desktopAvailableWidth
    height: mainController.isDesktop ? 800 : Screen.desktopAvailableHeight
    visible: true

    color: GStyle.backgroundColor

    function s(num) {
        return Utility.scaleByDpi(num)
    }

    function sr(num) {
        return Utility.scaleRoundByDpi(num)
    }

    Connections
    {
        target: Qt.application

        onStateChanged: {
            Qt.inputMethod.hide()
        }
    }

    Timer {
        interval: Static.internetCheckConnectionTimeInterval
        running: (sessionManager.connectionState === Enums.ConnectionState.ConnectionError)
        repeat: true
        onTriggered: sessionManager.ping()
    }

    MainStackView {
        id: mainStackView

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        height: Qt.inputMethod.keyboardRectangle.y > 0 && Qt.inputMethod.keyboardRectangle.y < mainWindow.height
                ? Qt.inputMethod.keyboardRectangle.y + Math.round(s(GStyle.footerHeight))
                : parent.height

        Behavior on height { NumberAnimation { duration: GStyle.keyboardAnimationDuration } }
    }

    onClosing: close.accepted = mainStackView.onClosingEvent()
}
