import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import "../../items" as Items

import com.gaiachain.style 1.0
import com.gaiachain.static 1.0

RowLayout {
    id: root

    property string qrCode
    readonly property int sectionLength: 4
    readonly property string separator: "-"
    readonly property real fontSize: s(GStyle.bigPixelSize)
    readonly property int sectionWidth: s(fontSize * 5)

    height: fontSize * 2
    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

    onVisibleChanged: qrPart1.forceActiveFocus()

    Items.GInput {
        id: qrPart1
        Layout.fillWidth: true
        Layout.maximumWidth: root.sectionWidth
        font.pixelSize: root.fontSize
        height: root.height
        borderWidth: 0
        padding: 0
        maximumLength: root.sectionLength
        inputMask: Static.qrSectionInputMask
        horizontalAlignment: Qt.AlignRight
        nextInput: qrPart2

        Keys.onRightPressed: {
            checkAndGoForward()
            cursorPosition = cursorPosition + 1
        }

        onTextChanged: checkAndGoForward()

        function checkAndGoBack() {
            // nothing
            return false
        }

        function checkAndGoForward() {
            if (cursorPosition === maximumLength) {
                moveToNextInput()
                qrPart2.cursorPosition = 0
            }
        }
    }

    Text {
        text: root.separator
        font.pixelSize: root.fontSize
        height: root.height
    }

    Items.GInput {
        id: qrPart2
        Layout.fillWidth: true
        Layout.maximumWidth: root.sectionWidth
        font.pixelSize: root.fontSize
        height: root.height
        borderWidth: 0
        padding: 0
        maximumLength: root.sectionLength
        inputMask: Static.qrSectionInputMask
        horizontalAlignment: Qt.AlignHCenter
        nextInput: qrPart3

        Keys.onLeftPressed: {
            checkAndGoBack()
            cursorPosition = cursorPosition - 1
        }

        Keys.onRightPressed: {
            checkAndGoForward()
            cursorPosition = cursorPosition + 1
        }

        onTextChanged: {
            if (checkAndGoBack() === false) {
                checkAndGoForward()
            }
        }

        function checkAndGoBack() {
            if (cursorPosition <= 0) {
                qrPart1.forceActiveFocus()
                qrPart1.cursorPosition = qrPart1.maximumLength
                return true
            }
            return false
        }

        function checkAndGoForward() {
            if (cursorPosition === maximumLength) {
                moveToNextInput()
                qrPart3.cursorPosition = 0
           }
        }
    }

    Text {
        text: root.separator
        font.pixelSize: root.fontSize
        height: root.height
    }

    Items.GInput {
        id: qrPart3
        Layout.fillWidth: true
        Layout.maximumWidth: root.sectionWidth
        font.pixelSize: root.fontSize
        height: root.height
        borderWidth: 0
        padding: 0
        maximumLength: root.sectionLength
        inputMask: Static.qrSectionInputMask
        horizontalAlignment: Qt.AlignLeft

        Keys.onLeftPressed: {
            checkAndGoBack()
            cursorPosition = cursorPosition - 1
        }

        onTextChanged: checkAndGoBack()

        function checkAndGoBack() {
            if (cursorPosition <= 0) {
                qrPart2.forceActiveFocus()
                qrPart2.cursorPosition = qrPart2.maximumLength
                return true
            }
            return false
        }

        function checkAndGoForward() {
            // Nothing
        }
    }
}
