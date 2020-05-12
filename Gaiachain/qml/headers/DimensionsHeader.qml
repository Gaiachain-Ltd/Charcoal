import QtQuick 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items
import "../headers" as Headers

Headers.GHeader {
    id: top

    property bool readOnly: false

    property alias heightText: inputHeight.text
    property alias lengthText: inputLength.text
    property alias widthText: inputWidth.text

    readonly property bool isEmpty: (heightText === Strings.empty)
                                    && (lengthText === Strings.empty)
                                    && (widthText === Strings.empty)
    property int borderWidth: sr(1)

    function clear() {
        heightText = Strings.empty
        lengthText = Strings.empty
        widthText = Strings.empty
    }

    widget: RowLayout {
        Layout.fillWidth: true
        Layout.leftMargin: top.margins
        Layout.rightMargin: top.margins

        Items.GInput {
            id: inputHeight

            Layout.fillWidth: true
            placeholderText: Strings.height

            readOnly: top.readOnly
            borderWidth: top.borderWidth
            focus: false
            backgroundColor: top.highlighted? GStyle.delegateHighlightColor : GStyle.backgroundColor
            color: top.highlighted? GStyle.fontHighlightColor : GStyle.textPrimaryColor
            font.bold: top.highlighted? true : false
            validator: DoubleValidator {
                bottom: 0.0
            }
        }

        Items.GInput {
            id: inputLength

            Layout.fillWidth: true
            placeholderText: Strings.length

            readOnly: top.readOnly
            borderWidth: top.borderWidth
            focus: false
            backgroundColor: top.highlighted? GStyle.delegateHighlightColor : GStyle.backgroundColor
            color: top.highlighted? GStyle.fontHighlightColor : GStyle.textPrimaryColor
            font.bold: top.highlighted? true : false
            validator: DoubleValidator {
                bottom: 0.0
            }
        }

        Items.GInput {
            id: inputWidth

            Layout.fillWidth: true
            placeholderText: Strings.width

            readOnly: top.readOnly
            borderWidth: top.borderWidth
            focus: false
            backgroundColor: top.highlighted? GStyle.delegateHighlightColor : GStyle.backgroundColor
            color: top.highlighted? GStyle.fontHighlightColor : GStyle.textPrimaryColor
            font.bold: top.highlighted? true : false
            validator: DoubleValidator {
                bottom: 0.0
            }
        }
    }
}
