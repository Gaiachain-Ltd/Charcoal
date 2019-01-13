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
        onStackViewBackToPage: {
            stackView.pop(stackView.find(function(item) {
                  return item.page === backPage
              }), getMode(immediate));
        }
    }

    function getMode(isImmediate) {
        return isImmediate ? StackView.Immediate : StackView.Transition
    }

    // handler for android back button
    function onClosingEvent() {
        if (stackView.depth === 0)
            return false
        var result = stackView.get(stackView.depth - 1).closeEventHandler()
        if (result === undefined)
            return false

        return result
    }

    // TO_DO add handling of calendar page separetly as it is heavy component!
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
    }
}
