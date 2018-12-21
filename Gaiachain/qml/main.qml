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
                property: "opacity"
                from: 0
                to:1
                duration: 2000
            }
        }
        pushExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to:0
                duration: 2000
            }
        }
        popEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to:1
                duration: 2000
            }
        }
        popExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to:0
                duration: 2000
            }
        }
    }

    footer: RegisterBar {
        height: 100
        onBarClicked: {
            stackView.pop()
        }
    }
}
