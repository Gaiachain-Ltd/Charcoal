import QtQuick 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items
import "../headers" as Headers

Headers.AbstractListHeader {
    id: top

    widget: RowLayout {
        Layout.fillWidth: true
        Layout.leftMargin: top.margins
        Layout.rightMargin: top.margins

        Repeater {
            id: repeater
            property alias readOnly: top.readOnly
            property alias backgroundColor: top.backgroundColor

            model: titles.length

            ColumnLayout {
                Layout.fillWidth: true

                Items.GText {
                    Layout.alignment: Qt.AlignHCenter

                    visible: summaryMode
                    text: titles[index]
                    color: highlighted? secondaryColor : mainColor
                    font.bold: highlighted? true : false
                }

                Items.GInput {
                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true

                    placeholderText: titles[index]
                    text: values[index]
                    onTextChanged: values[index] = text
                    horizontalAlignment: TextInput.AlignHCenter

                    readOnly: repeater.readOnly
                    borderWidth: summaryMode? 0 : sr(1)
                    focus: false
                    backgroundColor: repeater.backgroundColor
                    color: highlighted? secondaryColor : mainColor
                    font.bold: highlighted? true : false
                    validator: DoubleValidator {
                        bottom: 0.0
                    }
                }
            }
        }
    }
}
