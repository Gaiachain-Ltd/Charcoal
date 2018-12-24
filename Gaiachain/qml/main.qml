import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Window 2.2

ApplicationWindow
{
    id: mainWindow

    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight
    visible: true

    color: "white"

    function s(num) {
        return utility.scaleByDpi(num)
    }

    function sr(num) {
        return utility.scaleRoundByDpi(num)
    }

    header: NavigationHeader {
        visible: mainStackView.headerVisible
        height: s(150)
    }

    MainStackView {
        id: mainStackView
        anchors.fill: parent
    }

    footer: RegisterBar {
        visible: mainStackView.footerVisible
        height: s(100)
    }
}
