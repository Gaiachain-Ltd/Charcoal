import QtQuick 2.5
import QtQuick.Controls 2.4
import QtQuick.Window 2.2

ApplicationWindow
{
    id: mainWindow
    visible: true
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight
    color: "white"

    property var colors: ["violet", "gray", "pink", "magenta"]

    Component {
        id: page
        Rectangle {
            color: colors[stackView.depth % colors.length]
        }
    }

    header: NavigationHeader {
        height: 150
        onHeaderClicked: {
            if (stackView.depth % 2) {
                stackView.push("testComponents/CommodityRangeTestPage.qml")
            } else {
                stackView.push(page)
            }
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: page

        pushEnter: Transition {
            PropertyAnimation {
                property: "x"
                from: mainWindow.width
                to: 0
                duration: 350
            }
        }
        pushExit: Transition {
            PropertyAnimation {
                property: "x"
                from: 0
                to: -mainWindow.width
                duration: 350
            }
        }
        popEnter: Transition {
            PropertyAnimation {
                property: "x"
                from: -mainWindow.width
                to: 0
                duration: 350
            }
        }
        popExit: Transition {
            PropertyAnimation {
                property: "x"
                from: 0
                to: mainWindow.width
                duration: 350
            }
        }
    }

    footer: RegisterBar {
        height: 100
        onBarClicked: {
            if (stackView.depth > 0) {
                stackView.pop()
            }
        }
    }
}
