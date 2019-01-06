import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Window 2.2

import "./items" as Items

ApplicationWindow
{
    id: mainWindow

    width: isDesktop ? 480 : Screen.desktopAvailableWidth
    height: isDesktop ? 854 : Screen.desktopAvailableHeight
    visible: true

    color: "white"

    function s(num) {
        return utility.scaleByDpi(num)
    }

    function sr(num) {
        return utility.scaleRoundByDpi(num)
    }

    MainStackView {
        id: mainStackView
        anchors {
            fill: parent
            bottomMargin: Qt.inputMethod.keyboardRectangle.y > 0 ? Qt.inputMethod.keyboardRectangle.height : 0
        }
    }
}
