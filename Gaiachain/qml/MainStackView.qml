import QtQuick 2.11
import QtQuick.Controls 2.4

Item {
    property alias depth: stackView.depth
    property string initialPage: "qrc:/pages/ResourceChosingPage.qml"

    function pop() {
        stackView.pop()
    }

    function push(page) {
        stackView.push(page)
    }

    Connections {
        target: pageManager
        onPush: {
            stackView.push(url)
        }

        onPop: {
            stackView.pop()
        }

        onGoToInitial: {
            stackView.clear()
            stackView.push(initialPage, StackView.Immediate)
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: initialPage

        pushEnter: Transition {
            PropertyAnimation {
                property: "x"
                from: ApplicationWindow.window.width
                to: 0
                duration: 350
            }
        }
        pushExit: Transition {
            PropertyAnimation {
                property: "x"
                from: 0
                to: -ApplicationWindow.window.width
                duration: 350
            }
        }
        popEnter: Transition {
            PropertyAnimation {
                property: "x"
                from: -ApplicationWindow.window.width
                to: 0
                duration: 350
            }
        }
        popExit: Transition {
            PropertyAnimation {
                property: "x"
                from: 0
                to: ApplicationWindow.window.width
                duration: 350
            }
        }
    }
}
