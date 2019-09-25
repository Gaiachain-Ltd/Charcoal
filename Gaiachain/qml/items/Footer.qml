import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

Item {
    id: top

    implicitHeight: s(Style.footerHeight)

    Rectangle {
        id: background
        anchors.fill: parent
        color: Style.footerBackgroundColor
    }

    RowLayout {
        anchors {
            fill: parent
            margins: s(Style.normalMargin)
        }

        ConnectionStatusItem {
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredHeight: s(Style.buttonImageHeight)
            Layout.fillWidth: true

            connectionStatus: Enums.ConnectionStatus.ConnectionSuccessful   // TODO real connection status
        }

        ImageButton {
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredHeight: s(Style.buttonImageHeight)

            source: Style.refreshImgUrl
            palette.button: Style.unknownColor

            onClicked: refreshData()
        }
    }
}
