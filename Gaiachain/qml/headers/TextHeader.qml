import QtQuick 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items
import "../headers" as Headers

Headers.GHeader {
    id: root

    property alias text: textWidget.text

    property bool optional: false
    readonly property bool isEmpty: (text === Strings.empty)

    property bool forceBoldValue: false
    property int valueFontSize: s(GStyle.inputPixelSize)

    function clear() {
        text = Strings.empty
    }

    widget: Items.GText {
        id: textWidget

        Layout.fillWidth: true
        Layout.margins: root.margins
        Layout.leftMargin: extraLeftMargin? root.margins * 2 : root.margins

        focus: false
        color: secondaryTextColor
        font.bold: root.highlighted || forceBoldValue
        font.pixelSize: valueFontSize

        wrapMode: Text.WrapAnywhere
        horizontalAlignment: Text.AlignLeft
    }
}
