import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

import "../items" as Items
import "../components" as Components

Item {
    id: top

    implicitHeight: s(Style.footerHeight)

    Items.BlockMouseArea{}

    Rectangle {
        id: background
        anchors.fill: parent
        color: Style.footerBackgroundColor
    }

    RowLayout {
        anchors {
            fill: parent
            margins: s(Style.middleMargin)
        }

        Components.ConnectionStateItem {
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredHeight: s(Style.buttonImageHeight)
            Layout.fillWidth: true

            connectionState: sessionManager.connectionState
        }

        Components.CommunicationStateItem {
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredHeight: s(Style.buttonImageHeight)
            Layout.fillWidth: true

            communicationInProgress: dataManager.collectingData
        }
    }
}
