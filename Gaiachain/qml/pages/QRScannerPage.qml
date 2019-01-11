import QtQuick 2.11
import QtQuick.Layouts 1.11
import QtMultimedia 5.9
import QZXing 2.3

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

import "../items" as Items

/*
        Jak zeskanuje poprawny kod, to ramka zrobi się zielona (#96c354),
        a jak błędny, to czerwona (ff0000) i pojawi się napis "Scan failed",
        tak jak jest teraz, tylko czerwony.

        Oprócz tej zmiany w obydwu przypadkach (poprawny i niepoprawny kod)
        kamera zrobi zdjęcie zeskanowanego kodu i je wyświetli,
        a obok przycisku [ x ] (anuluj/zamknij),
        pojawi się nowa ikona do ponownego odpalenia trybu skanowania.
  */

BasePage {
    id: top

    function enterEventDetailsPage() {
        pageManager.enter(Enums.Page.EditableEventDetails)
    }

    function previewCapturedImage(result) {
        photoPreview.source = result.url
        photoPreview.visible = true
    }

    function parseScannedId(id) {
        if (scannedId.length == 0) {
            if (utility.validateId(id)) {
                scannedId = id
            } else {
                error = true
                console.warn("Wrong code content!", id)
            }
            grabImageOfCamera()
        }
    }

    function grabImageOfCamera() {
        if (!photoPreview.visible)
            videoOutput.grabToImage(previewCapturedImage)
    }

    function parseInputId() {
        if (scanInput.text.length > 0) {
            parseScannedId(scanInput.text)
        } else {
            retry()
        }
        scanInput.visible = false
        scanInput.focus = false
        Qt.inputMethod.hide()
    }

    function retry() {
        if (camera.cameraState !== Camera.ActiveState)
            camera.start()
        scanInput.visible = false
        error = false
        photoPreview.source = ""
        photoPreview.visible = false
        scannedId = ""
    }

    property string scannedId: ""
    property bool error: false

    Connections
    {
        target: pageManager
        onStackViewPop: camera.stop()
    }

    ColumnLayout
    {
        anchors.fill: parent
        spacing: 0

        Item {
            id: cameraContainer
            Layout.fillHeight: true
            Layout.fillWidth: true

            Rectangle
            {
                anchors.fill: parent
                color: Style.textGreyColor
            }

            Camera {
                id: camera
            }

            VideoOutput {
                id: videoOutput
                anchors.fill: parent
                source: camera
                filters: [zxingFilter]
                fillMode: VideoOutput.PreserveAspectCrop
                autoOrientation: true
            }

            Image {
                id: photoPreview
                visible: false
                anchors.fill: parent
            }

            Items.ItemBorder
            {
                id: scanBorder
                anchors.centerIn: parent
                width: Math.min(videoOutput.width, videoOutput.height) * 0.6
                height: width

                error: top.error
                finished: scannedId.length > 0
            }

            Row {
                id: buttonRow
                anchors {
                    top: parent.top
                    topMargin: s(Style.bigMargin)
                    right: parent.right
                    rightMargin: s(Style.bigMargin)
                }

                spacing: s(Style.bigMargin)
                layoutDirection: Qt.RightToLeft

                Items.ImageButton
                {
                    onClicked: pageManager.back()

                    backgroundColor: Style.backgroundShadowColor
                    source: Style.cancelImgUrl

                    padding: s(Style.smallMargin) * 2
                }

                Items.ImageButton
                {
                    onClicked: retry()

                    backgroundColor: Style.backgroundShadowColor
                    source: Style.relaodImgUrl

                    visible: error || scannedId.length > 0
                }
            }

            Items.ImageButton
            {
                text: Strings.logID
                backgroundColor: Style.backgroundShadowColor
                textColor: Style.textSecondaryColor
                showIcon: false
                textFont.bold: true

                width: textWidth + s(Style.bigMargin) * 1.75
                height: s(Style.buttonHeight) * 0.75

                anchors {
                    verticalCenter: buttonRow.verticalCenter
                    left: parent.left
                    leftMargin: s(Style.bigMargin)
                }
            }

            QZXingFilter {
                id: zxingFilter
                decoder {
                    enabledDecoders: QZXing.DecoderFormat_QR_CODE

                    onTagFound:  {
                        parseScannedId(tag)
                    }
                }
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: s(Style.footerHeight)

            RowLayout
            {
                anchors {
                    fill: parent
                    leftMargin: s(Style.normalMargin)
                    rightMargin: s(Style.normalMargin)
                    topMargin: s(Style.smallMargin)
                    bottomMargin: s(Style.smallMargin)
                }
                spacing: s(Style.bigMargin)

                Item
                {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Items.GenericInput
                    {
                        id: scanInput
                        onVisibleChanged: {
                            scanInput.text = ""
                        }

                        background.border.width: 0
                        anchors.fill: parent
                        visible: false
                        focus: true
                        placeholderText: Strings.typeId + "..."

                        additionalInputMethodHints: Qt.ImhDigitsOnly
                        input.maximumLength: utility.getScannedIdLength()
                        input.validator : RegExpValidator { regExp : /[0-9]+/ }

                        onMoveToNextInput: {
                            parseInputId()
                        }
                    }

                    Items.BasicText
                    {
                        id: scanLabel
                        visible: !scanInput.visible

                        anchors.fill: parent
                        text: {
                            if (error) {
                                return Strings.scanFailed
                            } else if (scannedId.length == 0) {
                                return Strings.scanning + "..."
                            } else {
                                return String("%1: <b>%2</b>").arg(Strings.id).arg(scannedId)
                            }
                        }

                        color: error ? Style.textErrorColor : Style.textPrimaryColor
                        horizontalAlignment: Text.AlignLeft
                        font.pixelSize: s(Style.pixelSize)
                        textFormat: Text.RichText
                    }
                }

                Items.ImageButton
                {
                    onClicked: {
                        if (scanInput.visible) {
                            parseInputId()
                        } else {
                            scanInput.visible = true
                            scanInput.focus = true
                            grabImageOfCamera()
                        }
                    }

                    padding: s(Style.smallMargin) * 1.25

                    enabled: !error && scannedId.length == 0

                    backgroundColor: enabled ? Style.buttonGreyColor : Style.disabledButtonGreyColor
                    source: scanInput.visible ? Style.qrCodeImgUrl : Style.keyboardImgUrl

                    Layout.preferredWidth: s(Style.buttonHeight)
                    Layout.preferredHeight: s(Style.buttonHeight)
                }

                Items.ImageButton
                {
                    onClicked: top.enterEventDetailsPage()

                    fillMode: Image.PreserveAspectFit

                    padding: s(Style.smallMargin) * 1.75

                    enabled: scannedId.length > 0

                    backgroundColor: enabled ? Style.buttonGreyColor : Style.disabledButtonGreyColor
                    source: Style.loginImgUrl

                    Layout.preferredWidth: s(Style.buttonHeight)
                    Layout.preferredHeight: s(Style.buttonHeight)
                }

            }
        }
    }

}
