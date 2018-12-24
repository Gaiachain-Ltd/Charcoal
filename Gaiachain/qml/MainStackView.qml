import QtQuick 2.11
import QtQuick.Controls 2.4

Item {
    property alias depth: stackView.depth
    property string initialPage: "qrc:/pages/ResourceChosingPage.qml"
    property int windowWidth: parent.width

    property bool headerVisible: true
    property bool footerVisible: true

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

        onCurrentItemChanged: {
            if (currentItem !== null)
                headerVisible = currentItem.headerVisible
            if (currentItem !== null)
                footerVisible = currentItem.footerVisible
        }

        pushEnter: Transition {
            PropertyAnimation {
                property: "x"
                from: windowWidth
                to: 0
                duration: 350
            }
        }
        pushExit: Transition {
            PropertyAnimation {
                property: "x"
                from: 0
                to: -windowWidth
                duration: 350
            }
        }
        popEnter: Transition {
            PropertyAnimation {
                property: "x"
                from: -windowWidth
                to: 0
                duration: 350
            }
        }
        popExit: Transition {
            PropertyAnimation {
                property: "x"
                from: 0
                to: windowWidth
                duration: 350
            }
        }
    }
}
