import QtQuick 2.12
import QtQuick.Controls 2.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items


CheckBox {
    id: top

    property alias elide: contentText.elide

    property string defaultColor: Style.inputBorderColor
    property string checkedColor: palette.text

    padding: s(Style.middleMargin)

    indicator: Rectangle {
        color: "transparent"

        anchors.fill: parent

        radius: s(Style.tinyMargin)
        antialiasing: true

        border {
            width: checked ? sr(Style.controlBoldBorderWidth) : sr(Style.controlDefaultBorderWidth)
            color: checked ? checkedColor : defaultColor
        }

        Behavior on border.color { PropertyAnimation { duration: Style.animationDuration; easing.type: Style.animationEasing } }
        Behavior on border.width { PropertyAnimation { duration: Style.animationDuration; easing.type: Style.animationEasing } }
    }

    contentItem: Items.BasicText {
        id: contentText

        font {
            pixelSize: s(Style.smallPixelSize)
            weight: top.checked ? Font.Bold : Font.Normal
        }

        text: top.text

        Behavior on font.weight { PropertyAnimation { duration: Style.animationDuration; easing.type: Style.animationEasing } }
    }
}
