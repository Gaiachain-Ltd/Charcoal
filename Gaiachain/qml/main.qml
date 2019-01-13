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

    Connections
    {
        target: Qt.inputMethod

        onKeyboardRectangleChanged: {
            if (Qt.inputMethod.keyboardRectangle.y > 0) {
                keyboardCorrectionTimer.restart()
            } else {
                resetValues()
            }
        }

        onVisibleChanged: {
            if (!Qt.inputMethod.visible)
                resetValues()
        }
    }

    function resetValues() {
        bottomMarginKeyboard = 0
        bottomMargin = 0
        headerTopMargin = 0
    }

    function calculateBottomMargin() {
        // Currently focused input bottomY position.
        var inputBottomY = mainWindow.activeFocusItem.mapToItem(null, mainWindow.activeFocusItem.x, mainWindow.activeFocusItem.y).y + mainWindow.activeFocusItem.height
        var keyboardY = Qt.inputMethod.keyboardRectangle.y
        var keyboardH = Qt.inputMethod.keyboardRectangle.height
        // If keyboard would hide input, page must be moved up.
        if (inputBottomY > keyboardY) {
            // Ignore if page position was already changed.
            if (bottomMarginKeyboard > 0)
                return
            // How high input is placed from bottom of the page.
            var screnRaiseDiff = (mainWindow.height - inputBottomY)
            // Calculate how much main window is raised from bottom
            // to move header lower to always display it.
            headerTopMargin = keyboardH - screnRaiseDiff
            // Move page up and hide footer.
            bottomMargin = screnRaiseDiff - s(Style.footerHeight)
        } else {
            // Ignore if page position was already changed.
            if (bottomMargin > 0)
                return
            // Change page height to fit all content and hide footer.
            bottomMarginKeyboard = keyboardH - s(Style.footerHeight)
        }
    }

    Timer
    {
        id: keyboardCorrectionTimer
        interval: 100

        onTriggered: {
            if (Qt.inputMethod.visible)
                calculateBottomMargin()
        }
    }

    property real bottomMargin: 0
    property real bottomMarginKeyboard: 0
    property real headerTopMargin: 0

    MainStackView {
        id: mainStackView
        anchors.fill: parent
    }

    onClosing: close.accepted = mainStackView.onClosingEvent()
}
