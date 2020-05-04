import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../../items" as Items

Item {
    id: top
    property bool communicationInProgress: false

    RowLayout {
        anchors.fill: parent
        spacing: s(GStyle.bigMargin)

        Items.GText {
            Layout.fillHeight: true
            Layout.fillWidth: true

            visible: communicationInProgress

            text: communicationInProgress ? Strings.loading + "..." : Strings.empty
            color: GStyle.unknownColor

            font.bold: true
            horizontalAlignment: Qt.AlignRight
        }

        Items.SpinnerItem {
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredHeight: s(GStyle.buttonImageSmallHeight)
            Layout.preferredWidth: s(GStyle.buttonImageSmallHeight)

            visible: communicationInProgress
        }

        Items.ImageButton {
            Layout.alignment: Qt.AlignVCenter | Qt.AlignRight

            visible: !communicationInProgress

            source: GStyle.refreshImgUrl
            palette.button: GStyle.unknownColor

            onClicked: retryConnection()
        }
    }
}
