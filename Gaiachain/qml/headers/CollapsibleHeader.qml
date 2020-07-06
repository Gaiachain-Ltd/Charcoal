import QtQuick 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items
import "../headers" as Headers
import "../components" as Components

Item {
    id: root

    property bool expanded: true
    property alias headerText: headerTextComponent.text
    property alias summary: summaryComponent.summary

    height: mainColumn.height

    ColumnLayout {
        id: mainColumn
        Layout.fillWidth: true

        MouseArea {
            id: headerRow
            Layout.fillWidth: true
            height: headerTextComponent.height

            onClicked: root.expanded = !root.expanded

            RowLayout {
                anchors {
                    top: parent.top
                    left: parent.left
                    right: parent.right
                }

                height: headerTextComponent.height

                Items.GText {
                    id: headerTextComponent
                    Layout.fillWidth: true
                }

                Image {
                    id: arrowImage
                    source: GStyle.rightBlackArrowImgUrl
                    rotation: root.expanded? 90 : 270
                    width: 30
                    height: 30
                    fillMode: Image.PreserveAspectFit
                }
            }
        }

        Components.Summary {
            id: summaryComponent
            Layout.fillWidth: true

            isSummaryMode: true
            visible: root.expanded
        }
    }
}
