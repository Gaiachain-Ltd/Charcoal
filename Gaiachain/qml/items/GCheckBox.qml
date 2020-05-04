import QtQuick 2.12
import QtQuick.Controls 2.12

import com.gaiachain.style 1.0

import "../items" as Items

CheckBox {
    id: top

    indicator: Rectangle {
        implicitWidth: s(GStyle.hugeMargin)
        implicitHeight: s(GStyle.hugeMargin)

        anchors {
            left: parent.left
            verticalCenter: parent.verticalCenter
        }

        antialiasing: true
        radius: s(GStyle.tinyMargin)
        border {
            color: GStyle.inputBorderColor
            width: sr(GStyle.controlDefaultBorderWidth)
        }

        gradient: Gradient {
            GradientStop { position: 0.0; color: GStyle.controlGradientStartColor }
            GradientStop { position: 1.0; color: GStyle.controlGradientStopColor }
        }

        Items.GText {
            anchors.centerIn: parent
            text: top.checked ? Strings.checkMark : Strings.empty
        }
    }

    contentItem: Items.GText {
        horizontalAlignment: Qt.AlignLeft
        leftPadding: (top.indicator.width + top.spacing)

        text: top.text
    }
}
