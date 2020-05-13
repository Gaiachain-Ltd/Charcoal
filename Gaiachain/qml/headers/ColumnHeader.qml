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
        Layout.leftMargin: top.margins
        Layout.rightMargin: top.margins

        Repeater {
            model: titles.length

            Items.GInput {
                id: inputHeight

                Layout.fillWidth: true
                placeholderText: titles[index]
                text: values[index]
                onTextChanged: values[index] = text

                readOnly: readOnly
                borderWidth: borderWidth
                focus: false
                backgroundColor: highlighted? GStyle.delegateHighlightColor : GStyle.backgroundColor
                color: highlighted? GStyle.fontHighlightColor : GStyle.textPrimaryColor
                font.bold: highlighted? true : false
                validator: DoubleValidator {
                    bottom: 0.0
                }
            }
        }
    }
}
