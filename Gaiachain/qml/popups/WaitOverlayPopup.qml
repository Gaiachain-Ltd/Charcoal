import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0

import "../items" as Items

GenericPopup
{
    id: top

    property string message

    topMargin: -(messageText.height / 2)

    background: Item {}
    contentItem: ColumnLayout {
        id: contentLayout
        readonly property real margins: s(Style.middleMargin)

        spacing: Style.none

        Items.SvgImage {
            id: spinner
            Layout.alignment: Qt.AlignHCenter
            Layout.margins: contentLayout.margins

            height: s(Style.buttonHeight)
            width: s(Style.buttonHeight)

            source: Style.spinnerImgUrl

            RotationAnimator {
                target: spinner
                from: 0
                to: 360
                duration: 1000
                running: top.visible
                loops: Animation.Infinite
            }
        }

        Items.BasicText {
            id: messageText
            Layout.alignment: Qt.AlignHCenter
            Layout.margins: contentLayout.margins

            visible: message.length
            text: top.message

            color: Style.textSecondaryColor
        }
    }

    Overlay.modal: Rectangle {
        color: Style.backgroundDarkShadowColor
    }
}
