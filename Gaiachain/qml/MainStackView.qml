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
            stackView.push(url, properties, getMode(immediate))
        }
        onStackViewPop: {
            stackView.pop(getMode(immediate))
        }
        onStackViewPopTo: {
            var backToPage = page
            stackView.pop(stackView.find(function(item) {
                  return item.page === backToPage
              }), getMode(immediate));

            // update properties
            for (var key in properties) {
                stackView.currentItem[key] = properties[key]
            }
        }
        onStackViewReplace: {
            stackView.replace(stackView.currentItem, url, properties, getMode(immediate))
        }

        onPopupManagerOpen: {
            popupManager.openPopup(url, properties)
        }
        onPopupManagerClose: {
            popupManager.closePopup()
        }
    }

    function getMode(isImmediate) {
        return isImmediate ? StackView.Immediate : StackView.Transition
    }

    // handler for android back button
    function onClosingEvent() {
        var result = false

        // cannot back popup
        if (!popupManager.depth && stackView.depth) {
            result = stackView.get(stackView.depth - 1).closeEventHandler()
            if (result === undefined) {
                result = false
            }
        }

        return result
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
        replaceEnter: Transition {
            PropertyAnimation {
                property: "x"
                from: windowWidth
                to: 0
                duration: Style.animationDuration
                easing.type: Style.animationEasing
            }
        }
        replaceExit: Transition {
            PropertyAnimation {
                property: "x"
                from: 0
                to: -windowWidth
                duration: Style.animationDuration
                easing.type: Style.animationEasing
            }
        }
    }

    QtObject {
        id: popupManager

        property int depth: stack.length
        property var components: ({})
        property var stack: []

        function openPopup(url, properties) {
            if (components[url] === undefined) {
                components[url] = Qt.createComponent(url)
                if (components[url].status === Component.Error) {
                    console.error("Error, while creating popup:", components[url].errorString())
                    return
                }
            }

            var popupComponent = components[url]
            var popup = popupComponent.createObject(mainWindow, properties)
            stack.push(popup)
            popup.open()
        }
        function closePopup() {
            var popup = stack.pop()
            popup.close()
            popup.destroy()
        }
    }
}
