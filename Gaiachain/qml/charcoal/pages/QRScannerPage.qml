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
import "../items" as CharcoalItems

Pages.GPage {
    enum StatusType {
        None,
        Scanning,
        ManualScan,
        Success,
        Warning,
        Error,
        Proceed
    }

    property bool infoVisible: true
    property bool infoDescriptionVisible: true

    property string infoText: "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec sagittis non nibh quis aliquam. Praesent vitae tempus velit. Donec id sem finibus lacus blandit tempus ac commodo elit. Donec sagittis consectetur nisl non eleifend."
    property var infoImages: [ GStyle.takePhotoUrl ]

    property string statusTextHeader: {
        switch (currentStatus) {
        case QRScannerPage.None:
            return Strings.empty
        case QRScannerPage.Scanning:
            return Strings.scanning
        case QRScannerPage.ManualScan:
            return Strings.empty
        case QRScannerPage.Success:
            return Strings.qrCodeScannedSuccessfully
        case QRScannerPage.Proceed:
            return Strings.doYouGoToTheNextOven
        case QRScannerPage.Warning:
            return Strings.qrCodeAlreadyAssigned
        case QRScannerPage.Error:
            return Strings.error
        }
    }

    property string statusTextValue: "LH4U-3YJT-LFND"

    property int currentStatus: QRScannerPage.None

    backgroundColor: GStyle.backgroundShadowColor

    id: top

    title: Strings.qrCode

    logoVisible: false
    showCloseButton: false

    function closeEventHandler() {
        pageManager.back()
        return false
    }

    function hideInfoOverlay() {
        infoVisible = false
        currentStatus = QRScannerPage.Scanning
        overlayTimer.stop()
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
            color: {
                switch (currentStatus) {
                case QRScannerPage.None:
                case QRScannerPage.Scanning:
                case QRScannerPage.ManualScan:
                    return GStyle.backgroundColor
                case QRScannerPage.Success:
                case QRScannerPage.Proceed:
                    return GStyle.statusGreen
                case QRScannerPage.Warning:
                    return GStyle.statusYellow
                case QRScannerPage.Error:
                    return GStyle.statusRed
                }
            }

            height: s(350)

            Row {
                anchors.verticalCenter: parent.top
                anchors.horizontalCenter: parent.horizontalCenter

                spacing: 40

                CharcoalItems.CharcoalRoundButton {
                    icon.source: GStyle.iconRetryScanRedUrl
                    visible: currentStatus === QRScannerPage.Error
                }

                CharcoalItems.CharcoalRoundButton {
                    icon.source: currentStatus === QRScannerPage.Warning?
                                     GStyle.iconScanAnotherYellowUrl
                                   : GStyle.iconScanAnotherGreenUrl

                    visible: (currentStatus === QRScannerPage.Warning
                              || currentStatus === QRScannerPage.Success)

                    onClicked: currentStatus = QRScannerPage.Success
                }

                CharcoalItems.CharcoalRoundButton {
                    icon.source: GStyle.iconListUrl

                    visible: (currentStatus === QRScannerPage.Warning
                              || currentStatus === QRScannerPage.Success)

                    onClicked: currentStatus = QRScannerPage.Warning
                }

                CharcoalItems.CharcoalRoundButton {
                    icon.source: GStyle.iconKeyboardUrl

                    visible: (currentStatus === QRScannerPage.Scanning
                              || currentStatus === QRScannerPage.Error)
                }

                CharcoalItems.CharcoalRoundButton {
                    icon.source: GStyle.logoutImgUrl

                    visible: (currentStatus === QRScannerPage.Scanning
                              || currentStatus === QRScannerPage.Success
                              || currentStatus === QRScannerPage.Warning
                              || currentStatus === QRScannerPage.Error)

                    onClicked: currentStatus = QRScannerPage.Error
                }

                CharcoalItems.CharcoalRoundButton {
                    icon.source: GStyle.iconYesUrl
                    visible: (currentStatus === QRScannerPage.Proceed
                              || currentStatus === QRScannerPage.ManualScan)
                }

                CharcoalItems.CharcoalRoundButton {
                    icon.source: GStyle.iconNoUrl
                    visible: (currentStatus === QRScannerPage.Proceed
                              || currentStatus === QRScannerPage.ManualScan)
                }
            }

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                property color textColor: currentStatus === QRScannerPage.None?
                                              GStyle.textPrimaryColor
                                            : GStyle.textSecondaryColor

                Item {
                    Layout.fillHeight: true
                    width: 1
                }

                Items.GText {
                    Layout.fillWidth: true
                    text: statusTextHeader
                    elide: Text.ElideNone
                    wrapMode: Text.WordWrap
                    color: parent.textColor
                    font.capitalization: Font.AllUppercase
                    font.pixelSize: s(GStyle.pixelSize)
                    verticalAlignment: Text.AlignBottom
                }

                Items.GText {
                    Layout.fillWidth: true
                    text: statusTextValue
                    elide: Text.ElideNone
                    wrapMode: Text.WordWrap
                    color: parent.textColor
                    font.capitalization: Font.AllUppercase
                    font.pixelSize: s(GStyle.bigPixelSize)
                    verticalAlignment: Text.AlignTop
                }

                Item {
                    Layout.fillHeight: true
                    width: 1
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
            spacing: s(GStyle.bigMargin)

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
                color: GStyle.textSecondaryColor
                elide: Text.ElideNone
                wrapMode: Text.WordWrap
                font.capitalization: Font.AllUppercase
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
}
