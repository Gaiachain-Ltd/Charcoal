import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items

Items.GenericHeader {
    id: top

    property alias model: comboBox.model

    property alias currentIndex: comboBox.currentIndex
    property alias currentText: comboBox.currentText
    property alias displayRole: comboBox.textRole
    property alias count: comboBox.count

    property alias placeholderText: comboBox.placeholderText

    signal activated(string activatedText)

    function selectItem(itemName) {
        var index = comboBox.find(itemName)

        if (index !== -1) {
            comboBox.currentIndex = index
        }
    }

    widget: Items.GenericComboBox {
        id: comboBox

        width: top.width
        enabled: top.enabled

        onActivated: top.activated(currentText)
    }
}
