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

        Items.SpinnerItem {
            Layout.alignment: Qt.AlignHCenter
            Layout.margins: contentLayout.margins

            Layout.preferredHeight: height
            Layout.preferredWidth: width
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
