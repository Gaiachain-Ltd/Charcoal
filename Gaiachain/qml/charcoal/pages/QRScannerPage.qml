import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11
import QtMultimedia 5.12
import QZXing 2.3

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.types 1.0
import com.gaiachain.static 1.0

import "../../common" as Common
import "../../headers" as Headers
import "../../pages" as Pages
import "../../items" as Items

Pages.GPage {
    property bool infoVisible: true
    property bool infoDescriptionVisible: true

    property string infoText: "default"
    property var infoImages: [ GStyle.takePhotoUrl ]

    property string statusTextHeader: "Top row"
    property string statusTextValue: "Bottom row"

    id: top

    title: Strings.qrCode

    logoVisible: false
    showCloseButton: false

    enum StatusType {
        Normal,
        Green,
        Red,
        Yellow
    }

    function closeEventHandler() {
        pageManager.back()
        return false
    }

    Component.onCompleted: {
        if (typeof(AndroidPermissionsHandler) !== "undefined") {
            AndroidPermissionsHandler.requestPermission(AndroidPermissionsHandler.Camera)
        }
    }

    function parseScannedId(id) {
        if (Utility.validateId(id)) {
            scannedId = Utility.formatRawId(id)
            scanStatus = Enums.QRScanStatus.Success
        } else {
            scannedId = id
            scanStatus = Enums.QRScanStatus.Failed
            console.warn("Wrong code content", id)
        }
    }

    ColumnLayout {
        id: column

        anchors.fill: parent

        spacing: 0

        // Camera
        Camera {
            id: camera

            captureMode: Camera.CaptureVideo
            focus {
                focusMode: CameraFocus.FocusContinuous
                focusPointMode: CameraFocus.FocusPointAuto
            }
        }

        QZXingFilter {
            id: zxingFilter

            property real normalizedScanSize: Static.normalizedScanSize
            property real normalizedScanPos: (1.0 - normalizedScanSize) * 0.5

            captureRect: {
                videoOutput.contentRect
                videoOutput.sourceRect

                return videoOutput.mapRectToSource(
                            videoOutput.mapNormalizedRectToItem(
                                Qt.rect(normalizedScanPos,
                                        normalizedScanPos,
                                        normalizedScanSize,
                                        normalizedScanSize)
                                )
                            )
            }

            decoder {
                enabledDecoders: QZXing.DecoderFormat_QR_CODE
                onTagFound: {
                    if (scanStatus === Enums.QRScanStatus.Scanning && !qrStatus.manual) {
                        parseScannedId(tag)
                    }
                }
            }
        }

        VideoOutput {
            id: videoOutput

            Layout.fillWidth: true
            Layout.fillHeight: true

            source: camera
            filters: [zxingFilter]
            fillMode: VideoOutput.PreserveAspectCrop
            autoOrientation: true
        }

        // Buttons & status
        // TODO: make it into an independent component?
        Rectangle {
            id: statusRect

            Layout.fillWidth: true
            color: "green"
            height: s(350)

            /*
             Buttons:
             * add (color)
             * list
             * exit
             * retry (color)
             * type manually
             * yes (green)
             * no (red)

             Colors:
             * white
             * green
             * yellow
             * red
             */

            Row {
                anchors.verticalCenter: parent.top
                anchors.horizontalCenter: parent.horizontalCenter

                //anchors.left: parent.left
                //anchors.right: parent.right

                height: s(100)
                spacing: s(40)

                RoundButton {
                    text: "R" // Retry
                    visible: false
                }

                RoundButton {
                    text: "A" // Add
                    visible: true
                }

                RoundButton {
                    text: "L" // List
                    visible: true
                }

                RoundButton {
                    text: "K" // Keyboard - type manually
                    visible: false
                }

                RoundButton {
                    text: "E" // Exit
                    visible: true
                }

                RoundButton {
                    text: "Y" // Yes
                    visible: false
                }

                RoundButton {
                    text: "N" // No
                    visible: false
                }
            }

            ColumnLayout {
                anchors.fill: parent
                anchors.topMargin: s(GStyle.bigMargin)
                spacing: s(GStyle.smallMargin)

                Items.GText {
                    Layout.fillWidth: true
                    text: statusTextHeader
                    elide: Text.ElideNone
                    wrapMode: Text.WordWrap
                    color: GStyle.textSecondaryColor
                    font.capitalization: Font.AllUppercase
                }

                Items.GText {
                    Layout.fillWidth: true
                    text: statusTextValue
                    elide: Text.ElideNone
                    wrapMode: Text.WordWrap
                    color: GStyle.textSecondaryColor
                    font.capitalization: Font.AllUppercase
                }
            }
        }
    }

    Rectangle {
        id: infoOverlay
        anchors.fill: parent

        color: GStyle.backgroundShadowColor
        visible: infoVisible

        ColumnLayout {
            id: infoLayout
            anchors.fill: parent

            Item {
                Layout.fillHeight: true
                width: 1
            }

            Image {
                Layout.alignment: Qt.AlignHCenter
                source: GStyle.infoUrl
                visible: infoDescriptionVisible
            }

            Items.GText {
                Layout.fillWidth: true
                text: infoText
                elide: Text.ElideNone
                wrapMode: Text.WordWrap
                visible: infoDescriptionVisible
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter

                Repeater {
                    model: infoImages

                    Image {
                        Layout.alignment: Qt.AlignHCenter
                        source: modelData
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
            onClicked: infoVisible = false
        }

        Timer {
            interval: 5000 // TODO: move to GStyle
            running: true
            repeat: false
            onTriggered: infoVisible = false
        }
    }
}
