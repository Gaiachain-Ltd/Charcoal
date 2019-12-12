import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items

Items.GenericHeader {
    id: top

    property alias placeholderText: description.placeholderText
    property alias inputText: description.inputText

    widget: Items.GenericTextArea {
        id: description

        Layout.fillWidth: true
        Layout.maximumHeight: s(Style.genericTextAreaHeight)
        Layout.minimumHeight: s(Style.genericTextAreaHeight)
    }
}

