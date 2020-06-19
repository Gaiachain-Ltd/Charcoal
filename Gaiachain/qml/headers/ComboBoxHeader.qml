import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items
import "../headers" as Headers

Headers.GHeader {
    id: top

    property alias filteredModel: comboBox.model
    property alias model: comboBox.sourceModel
    property alias currentIndex: comboBox.currentIndex
    property alias currentText: comboBox.currentText
    property alias displayRole: comboBox.textRole
    property alias count: comboBox.count
    property alias optional: comboBox.optional

    property alias placeholderText: comboBox.placeholderText

    property alias footerVisible: comboBox.footerVisible
    property alias footerText: comboBox.footerText

    signal activated(var index)
    signal footerClicked()

    widget: Items.FilterComboBox {
        id: comboBox

        Layout.fillWidth: true
        Layout.leftMargin: top.margins
        Layout.rightMargin: top.margins

        enabled: top.enabled

        onActivated: top.activated(index)
        onFooterClicked: top.footerClicked()
    }
}
