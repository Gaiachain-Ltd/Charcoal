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

    Layout.fillWidth: true

    property bool expanded: true
    property alias headerText: headerTextComponent.text
    property var summary

    height: mainColumn.height

    ColumnLayout {
        id: mainColumn

        width: parent.width

        MouseArea {
            id: headerRow
            Layout.fillWidth: true
            height: headerTextComponent.height * 1.5

            onClicked: root.expanded = !root.expanded

            RowLayout {
                anchors {
                    top: parent.top
                    left: parent.left
                    right: parent.right
                    topMargin: s(GStyle.bigMargin)
                }

                height: headerTextComponent.height

                Items.GText {
                    id: headerTextComponent
                    Layout.fillWidth: true
                    Layout.leftMargin: s(GStyle.bigMargin)

                    font.pixelSize: s(GStyle.titlePixelSize)
                    font.bold: true
                    horizontalAlignment: Text.AlignLeft
                }

                Image {
                    id: arrowImage
                    Layout.rightMargin: s(GStyle.bigMargin)

                    source: GStyle.rightBlackArrowImgUrl
                    rotation: root.expanded? 90 : 270
                    width: 30
                    height: 30
                    fillMode: Image.PreserveAspectFit
                }
            }
        }

        Loader {
            id: summaryLoader
            Layout.fillWidth: true

            visible: root.expanded
            source: root.expanded? "qrc:/components/Summary.qml" : ""

            onLoaded: {
                item.isSummaryMode = true
                item.summary = root.summary
            }
        }
    }
}
