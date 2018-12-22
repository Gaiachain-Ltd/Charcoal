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
            stackView.push(page)
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: page

        pushEnter: Transition {
            PropertyAnimation {
                property: "x"
                from: -mainWindow.width
                to: 0
                duration: 2000
                easing.type: Easing.OutCubic
            }
        }
        pushExit: Transition {
            PropertyAnimation {
                property: "x"
                from: 0
                to: mainWindow.width
                duration: 2000
                easing.type: Easing.OutCubic
            }
        }
        popEnter: Transition {
            PropertyAnimation {
                property: "x"
                from: -mainWindow.width
                to: 0
                duration: 2000
                easing.type: Easing.OutCubic
            }
        }
        popExit: Transition {
            PropertyAnimation {
                property: "x"
                from: 0
                to: mainWindow.width
                duration: 2000
                easing.type: Easing.OutCubic
            }
        }
    }

    property var tmpItem
    footer: RegisterBar {
        height: 100
        onBarClicked: {
            tmpItem = stackView.pop()
        }
    }
}
