import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items
import "../headers" as Headers

Headers.GHeader {
    id: top

    property alias placeholderText: description.placeholderText
    property alias inputText: description.inputText

    widget: Items.GTextArea {
        id: description

        Layout.fillWidth: true
        Layout.leftMargin: top.margins
        Layout.rightMargin: top.margins
        Layout.maximumHeight: s(GStyle.genericTextAreaHeight)
        Layout.minimumHeight: s(GStyle.genericTextAreaHeight)
    }
}

