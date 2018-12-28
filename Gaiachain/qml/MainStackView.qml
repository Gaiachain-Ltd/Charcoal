import QtQuick 2.11
import QtQuick.Controls 2.4

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

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
        onPush: stackView.push(url, properites)
        onPop: stackView.pop()
        onGoToInitial: stackView.pop(null)
    }

    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: initialPage

        Component.onCompleted: {
            //TO_DO temporarly enter calendar on start
            pageManager.enterPage(Enums.Page.ViewType, {})
            pageManager.enterPage(Enums.Page.Calendar, {})
        }

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
                duration: Style.animationDuration
                easing.type: Style.animationEasing
            }
        }
        pushExit: Transition {
            PropertyAnimation {
                property: "x"
                from: 0
                to: -windowWidth
                duration: Style.animationDuration
                easing.type: Style.animationEasing
            }
        }
        popEnter: Transition {
            PropertyAnimation {
                property: "x"
                from: -windowWidth
                to: 0
                duration: Style.animationDuration
                easing.type: Style.animationEasing
            }
        }
        popExit: Transition {
            PropertyAnimation {
                property: "x"
                from: 0
                to: windowWidth
                duration: Style.animationDuration
                easing.type: Style.animationEasing
            }
        }
    }
}
