import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as CharcoalItems
import "../../headers" as Headers

Headers.GHeader {
    id: top

    property alias model: comboBox.model
    property alias placeholderText: comboBox.placeholderText
    property alias currentText: comboBox.text
    property alias popupTitle: comboBox.popupTitle

    property alias checkIcon: comboBox.checkIcon
    property alias delegateTextColor: comboBox.delegateTextColor

    property alias multiSelect: comboBox.multiSelect

    property alias readOnly: comboBox.readOnly

    widget: CharcoalItems.CharcoalComboBox {
        id: comboBox

        Layout.fillWidth: true
        Layout.leftMargin: top.margins
        Layout.rightMargin: top.margins

        enabled: top.enabled
    }
}
