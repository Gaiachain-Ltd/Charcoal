import QtQuick 2.14

import "../../items" as Items

import com.gaiachain.style 1.0
import com.gaiachain.static 1.0

Items.GInput {
    readonly property int sectionLength: 4
    readonly property real fontSize: s(GStyle.titlePixelSize)
    readonly property int sectionWidth: s(fontSize * 4.5)
    readonly property int sectionWidthWide: s(fontSize * 7.5)
    property Item previousInput: null

    anchors.verticalCenter: parent.verticalCenter

    height: fontSize * 2
    width: contentWidth > sectionWidth? sectionWidthWide : sectionWidth
    borderWidth: 0
    padding: 0
    maximumLength: sectionLength

    backgroundColor: GStyle.blank

    font {
        pixelSize: fontSize
        capitalization: Font.AllUppercase
        letterSpacing: s(1)
    }

    horizontalAlignment: {
        if (previousInput && nextInput) {
            return Qt.AlignHCenter
        }

        if (previousInput) {
            return Qt.AlignLeft
        }

        if (nextInput) {
            return Qt.AlignRight
        }
    }

    verticalAlignment: Qt.AlignVCenter

    validator: RegularExpressionValidator {
        id: qrValidator
        regularExpression: /[0-9A-Za-z]+/
    }

    focus: false
    overwriteMode: true
    cursorDelegate: focus? cursor : emptyCursor

    Keys.onLeftPressed: {
        checkAndGoBack()
        cursorPosition = cursorPosition - 1
    }

    Keys.onRightPressed: {
        checkAndGoForward()
        cursorPosition = cursorPosition + 1
    }

    onTextChanged: {
        if (text.length === 0 && checkAndGoBack()) {
            return
        }

        checkAndGoForward()
    }

    function checkAndGoBack() {
        if (previousInput === null) {
            return false
        }

        if (cursorPosition <= 0) {
            previousInput.forceActiveFocus()
            previousInput.cursorPosition = previousInput.maximumLength
            return true
        }
        return false
    }

    function checkAndGoForward() {
        if (nextInput === null) {
            return false
        }

        if (cursorPosition === maximumLength) {
            nextInput.forceActiveFocus()
            nextInput.cursorPosition = 0
            return true
        }

        return false
    }

    Component {
        id: cursor

        Rectangle {
            height: root.fontSize
            width: s(1)
            color: GStyle.textPrimaryColor
        }
    }

    Component {
        id: emptyCursor

        Item {}
    }
}
