import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11
import QtMultimedia 5.12
import QZXing 2.3

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.static 1.0

import "../../items" as Items

Rectangle {
    id: infoOverlay

    color: GStyle.backgroundShadowColor

    property alias images: repeater.model
    property alias text: infoText.text

    signal hidden()

    function hideInfoOverlay() {
        overlayTimer.stop()
        hidden()
    }

    onVisibleChanged: {
        if (visible === true) {
            overlayTimer.start()
        }
    }

    ColumnLayout {
        id: infoLayout
        anchors.fill: parent
        spacing: 2 * s(GStyle.bigMargin)

        Item {
            Layout.fillHeight: true
            width: 1
        }

        Image {
            Layout.alignment: Qt.AlignHCenter
            source: GStyle.infoUrl
        }

        Items.GText {
            id: infoText
            Layout.fillWidth: true
            color: GStyle.textSecondaryColor
            elide: Text.ElideNone
            wrapMode: Text.WordWrap
            font.capitalization: Font.AllUppercase
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: s(GStyle.bigMargin)

            Repeater {
                id: repeater

                ColumnLayout {
                    spacing: 2 * s(GStyle.bigMargin)

                    Image {
                        Layout.alignment: Qt.AlignHCenter
                        source: modelData
                    }
                }
            }
        }

        Item {
            Layout.fillHeight: true
            width: 1
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: hideInfoOverlay()
    }

    Timer {
        id: overlayTimer
        interval: GStyle.qrOverlayInterval
        running: true
        repeat: false
        onTriggered: hideInfoOverlay()
    }
}
