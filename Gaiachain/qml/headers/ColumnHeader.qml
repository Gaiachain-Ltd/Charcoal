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
        spacing: 0

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
                    height: font.pixelSize + 4
                }

                Items.GText {
                    id: inputHeight

                    Layout.fillWidth: true
                    horizontalAlignment: TextInput.AlignRight

                    text: values[index]
                    onTextChanged: values[index] = text

                    padding: summaryMode? s(GStyle.picoMargin) : s(GStyle.bigMargin)

                    focus: false
                    color: highlighted? secondaryTextColor : mainColor
                    font.bold: false

                    elide: Text.ElideNone
                    wrapMode: Text.WordWrap
                }
            }
        }
    }
}
