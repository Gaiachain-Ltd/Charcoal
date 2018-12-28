import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

import "../items" as Items

import com.gaiachain.style 1.0

Item {
    id: top
    property bool footerVisible: true
    property bool headerVisible: true

    property color backgroundColor: "white"

    default property alias content: pageContent.data

    ColumnLayout {
        anchors.fill: parent

        spacing: 0

        Items.NavigationHeader {
            Layout.fillWidth: true
            Layout.preferredHeight: s(Style.headerHeight)
            visible: top.headerVisible
        }

        Item {
            id: pageContent
            Layout.fillWidth: true
            Layout.fillHeight: true

            Rectangle {
                id: background
                anchors.fill: parent
                color: top.backgroundColor
            }
        }

        Items.Footer {
            Layout.fillWidth: true
            Layout.preferredHeight: s(Style.footerHeight)
            visible: top.footerVisible
        }
    }
}
