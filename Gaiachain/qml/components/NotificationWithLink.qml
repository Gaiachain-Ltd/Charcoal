import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.types 1.0

import "../items" as Items

Rectangle {
    readonly property color textColor: GStyle.textSecondaryColor
    property string headerText: "default text, move along"
    property string text: "default text, move along"
    property string redirectText: "default page"
    property int redirectPage: Enums.Page.InvalidPage

    id: root

    color: GStyle.harvestColor

    function hide() {
        root.visible = false
    }

    onVisibleChanged: {
        if (visible) {
            timer.start()
        }
    }

    Timer {
        id: timer
        interval: GStyle.notificationPopupOpenedDefaultInterval
        repeat: false
        onTriggered: hide()
    }

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Items.GText {
                Layout.fillWidth: true
                color: textColor
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
            Layout.fillHeight: true
            width: 1
        }

        Items.GText {
            Layout.fillWidth: true
            color: textColor
        }

        Items.GText {
            Layout.fillWidth: true
            color: textColor
            text: Strings.redirect.arg(redirectText)

            Rectangle {
                anchors.fill: parent
                border {
                    width: s(2)
                    color: textColor
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    pageManager.enterPage(redirectPage)
                    hide()
                }
            }
        }
    }
}
