import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Window 2.2

ApplicationWindow
{
    id: mainWindow
    visible: true
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight

    color: "white"

    function s(num) {
        return utility.scaleByDpi(num)
    }

    function sr(num) {
        return utility.scaleRoundByDpi(num)
    }

    header: NavigationHeader {
        height: s(150)
    }

    MainStackView {
        id: mainStackView
        anchors.fill: parent
    }

    footer: RegisterBar {
        height: s(100)
    }
}
