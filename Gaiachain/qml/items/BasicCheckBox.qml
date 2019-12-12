import QtQuick 2.12
import QtQuick.Controls 2.12

import com.gaiachain.style 1.0

import "../items" as Items

CheckBox {
    id: top

    indicator: Rectangle {
        implicitWidth: s(Style.hugeMargin)
        implicitHeight: s(Style.hugeMargin)

        anchors {
            left: parent.left
            verticalCenter: parent.verticalCenter
        }

        antialiasing: true
        radius: s(Style.tinyMargin)
        border {
            color: Style.inputBorderColor
            width: sr(Style.controlDefaultBorderWidth)
        }

        gradient: Gradient {
            GradientStop { position: 0.0; color: Style.controlGradientStartColor }
            GradientStop { position: 1.0; color: Style.controlGradientStopColor }
        }

        Items.BasicText {
            anchors.centerIn: parent
            text: top.checked ? Strings.checkMark : Strings.empty
        }
    }

    contentItem: Items.BasicText {
        horizontalAlignment: Qt.AlignLeft
        leftPadding: (top.indicator.width + top.spacing)

        text: top.text
    }
}
