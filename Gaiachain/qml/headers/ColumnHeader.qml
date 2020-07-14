import QtQuick 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items
import "../headers" as Headers

Headers.AbstractListHeader {
    id: top

    widget: ColumnLayout {
        Layout.fillWidth: true
        Layout.leftMargin: extraLeftMargin? top.margins * 2 : top.margins
        Layout.rightMargin: top.margins

        Repeater {
            id: repeater
            property alias readOnly: top.readOnly
            property alias backgroundColor: top.backgroundColor

            model: titles.length

            RowLayout {

                Items.GText {
                    Layout.alignment: Qt.AlignLeft

                    visible: summaryMode
                    text: titles[index]
                    color: highlighted? secondaryTextColor : mainColor
                    font.bold: highlighted? true : false
                }

                Items.GInput {
                    id: inputHeight

                    Layout.fillWidth: true
                    horizontalAlignment: TextInput.AlignRight

                    placeholderText: titles[index]
                    text: values[index]
                    onTextChanged: values[index] = text

                    readOnly: repeater.readOnly
                    borderWidth: summaryMode? 0 : sr(1)
                    focus: false
                    backgroundColor: repeater.backgroundColor
                    color: highlighted? secondaryTextColor : mainColor
                    font.bold: false
                    validator: DoubleValidator {
                        bottom: 0.0
                    }
                }
            }
        }
    }
}
