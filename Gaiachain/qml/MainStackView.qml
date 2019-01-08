import QtQuick 2.11
import QtQuick.Controls 2.4

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

Item {
    property alias depth: stackView.depth
    property int windowWidth: parent.width

    Connections {
        target: pageManager

        onStackViewPush: {
            var keepPrevPageVisible = false
            if (properites.isPopup)
                keepPrevPageVisible = true
            stackView.push(url, properites, getMode(immediate))
            if (keepPrevPageVisible && stackView.depth > 1)
                stackView.get(stackView.depth - 2).visible = true
        }
        onStackViewPop: {
            stackView.pop(getMode(immediate))
        }
        onStackViewBackToInitial: {
            stackView.pop(null, getMode(immediate))
        }
        onStackViewBackToPage: {
            stackView.pop(stackView.find(function(item) {
                  return item.page === backPage
              }));
        }
    }

    function getMode(isImmediate) {
        return isImmediate ? StackView.Immediate : StackView.Transition
    }

    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: pageManager.getInitialPageUrl()

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

        Component.onCompleted: {
//            pageManager.push(Enums.Page.Login, {}, true)
        }
    }
}
