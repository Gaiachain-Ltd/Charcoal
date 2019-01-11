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
        // input global Y position
        var itemY = mainWindow.activeFocusItem.mapToItem(null, mainWindow.activeFocusItem.x, mainWindow.activeFocusItem.y).y
        var keyboardY = Qt.inputMethod.keyboardRectangle.y
        var keyboardH = Qt.inputMethod.keyboardRectangle.height
        // if keyboard would hide input, page must be moved up
        if (itemY + mainWindow.activeFocusItem.height > keyboardY) {
            if (bottomMarginKeyboard > 0)
                return
            var screnRaiseDiff = (mainWindow.height - itemY - mainWindow.activeFocusItem.height)
            // calculating how must lower header
            // to be visible when keyboard is displayed
            headerTopMargin = keyboardH - screnRaiseDiff
            bottomMargin = screnRaiseDiff - s(Style.footerHeight)
        } else {
            // input is above keyboard so
            // there is no need to move page
            // just change page height to fit all content
            // above keyboard)
            if (bottomMargin > 0)
                return
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

    // bottom margin to move page up
    // when keyboard is displayed
    // nad input would be hidden behind keyboard
    property real bottomMargin: 0
    // height change when input is above keyboard
    // and there is no need to move page up
    property real bottomMarginKeyboard: 0
    // top margin for header to always display it
    // when keyboard is showed
    property real headerTopMargin: 0

    MainStackView {
        id: mainStackView
        anchors.fill: parent
    }
}
