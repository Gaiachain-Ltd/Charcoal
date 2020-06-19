import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.types 1.0

import "../items" as Items

GPopup {
    id: root

    readonly property color textColor: GStyle.textSecondaryColor
    property alias headerText: headerItem.text
    property alias text: textItem.text
    property alias redirectText: redirectItem.text
    property int redirectPage: Enums.Page.InvalidPage

    property bool isTimed: false

    leftMargin: 0
    rightMargin: 0
    bottomMargin: 0

    width: parent.width
    height: parent.height * .4

    anchors.centerIn: null

    x: 0
    y: parent.height - height

    function hide() {
        pageManager.closePopup()
    }

    onVisibleChanged: {
        if (isTimed && visible) {
            timer.start()
        }
    }

    Timer {
        id: timer
        interval: GStyle.notificationPopupOpenedDefaultInterval
        repeat: false
        onTriggered: hide()
    }

    contentItem: ColumnLayout {
        anchors.margins: s(GStyle.bigMargin)
        spacing: s(GStyle.bigMargin)

        RowLayout {
            Items.GText {
                id: headerItem
                Layout.fillWidth: true
                color: textColor
                elide: Text.ElideNone
                wrapMode: Text.WordWrap
                font.pixelSize: s(GStyle.titlePixelSize)
                horizontalAlignment: Text.AlignLeft
            }

            Image {
                source: GStyle.closeImgUrl

                MouseArea {
                    anchors.fill: parent
                    onClicked: hide()
                }
            }
        }

        Item {
            // spacer
            height: s(GStyle.bigMargin)
            width: 1
        }

        Items.GText {
            id: textItem
            Layout.fillWidth: true
            color: textColor
            elide: Text.ElideNone
            wrapMode: Text.WordWrap
            font.pixelSize: s(GStyle.titlePixelSize)
            horizontalAlignment: Text.AlignLeft
        }

        Rectangle {
            Layout.fillWidth: true
            color: "transparent"
            radius: 5

            height: redirectItem.contentHeight + (2 * s(GStyle.middleMargin))

            border {
                width: 2
                color: textColor
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    hide()
                    pageManager.enter(redirectPage)
                }
            }

            Items.GText {
                id: redirectItem

                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    right: arrowImage.left
                    margins: s(GStyle.middleMargin)
                }

                color: textColor
                text: Strings.redirect.arg(redirectText)
                font.pixelSize: s(GStyle.pixelSize)
                horizontalAlignment: Text.AlignLeft
            }

            Image {
                id: arrowImage

                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    margins: s(GStyle.middleMargin)
                }

                source: GStyle.rightArrowImgUrl
            }
        }
    }

    background: Rectangle {
        color: GStyle.harvestColor
    }
}
