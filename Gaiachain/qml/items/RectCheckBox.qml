import QtQuick 2.12
import QtQuick.Controls 2.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items


CheckBox {
    id: top

    property alias elide: contentText.elide

    property string defaultColor: GStyle.inputBorderColor
    property string checkedColor: palette.text

    padding: s(GStyle.middleMargin)

    indicator: Rectangle {
        color: "transparent"

        anchors.fill: parent

        radius: s(GStyle.tinyMargin)
        antialiasing: true

        border {
            width: checked ? sr(GStyle.controlBoldBorderWidth) : sr(GStyle.controlDefaultBorderWidth)
            color: checked ? checkedColor : defaultColor
        }

        Behavior on border.color { PropertyAnimation { duration: GStyle.animationDuration; easing.type: GStyle.animationEasing } }
        Behavior on border.width { PropertyAnimation { duration: GStyle.animationDuration; easing.type: GStyle.animationEasing } }
    }

    contentItem: Items.GText {
        id: contentText

        font {
            pixelSize: s(GStyle.smallPixelSize)
            weight: top.checked ? Font.Bold : Font.Normal
        }

        text: top.text

        Behavior on font.weight { PropertyAnimation { duration: GStyle.animationDuration; easing.type: GStyle.animationEasing } }
    }
}
