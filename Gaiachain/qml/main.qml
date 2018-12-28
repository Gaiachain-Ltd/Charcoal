import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Window 2.2

import "./items" as Items

ApplicationWindow
{
    id: mainWindow

    width: isDesktop ? 300 : Screen.desktopAvailableWidth
    height: isDesktop ? 500 : Screen.desktopAvailableHeight
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
        anchors.fill: parent
    }
}
