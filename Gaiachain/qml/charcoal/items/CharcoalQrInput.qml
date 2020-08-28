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

        onCursorPositionChanged: {
            console.log("QRP1 pos", cursorPosition)
            if (cursorPosition === maximumLength) {
                qrPart2.forceActiveFocus()
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

        onCursorPositionChanged: {
            console.log("QRP2 pos", cursorPosition)
            if (cursorPosition === 0) {
                qrPart1.forceActiveFocus()
            } else if (cursorPosition === maximumLength) {
                qrPart3.forceActiveFocus()
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

        onCursorPositionChanged: {
            console.log("QRP3 pos", cursorPosition)
            if (cursorPosition === 0) {
                qrPart2.forceActiveFocus()
            }
        }
    }
}
