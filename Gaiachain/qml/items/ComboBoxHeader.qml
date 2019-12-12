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
    property alias optional: comboBox.optional

    property alias placeholderText: comboBox.placeholderText

    property alias footerVisible: comboBox.footerVisible
    property alias footerText: comboBox.footerText

    signal footerClicked()

    signal activated(string activatedText)

    function selectItem(itemName) {
        var index = comboBox.find(itemName)

        if (index !== -1) {
            comboBox.currentIndex = index
        }
    }

    widget: Items.GenericComboBox {
        id: comboBox

        Layout.fillWidth: true

        onActivated: top.activated(currentText)

        onFooterClicked: top.footerClicked()
    }
}
