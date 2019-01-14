import QtQuick 2.11
import QtQuick.Layouts 1.11
import QtMultimedia 5.9
import QZXing 2.3

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

import "../items" as Items

BasePage {
    id: top

    function enterEventDetailsPage() {
        var attributes = userManager.userData

        attributes["action"] = scannedIdAction
        attributes["timestamp"] = Number(new Date())
        attributes["shipmentId"] = scannedId

        pageManager.enter(Enums.Page.EditableEventDetails, { "attributes": attributes })
    }

    function previewCapturedImage(result) {
        photoPreview.source = result.url
        photoPreview.visible = true
    }

    function parseScannedId(id) {
        if (scannedId.length == 0) {
            if (utility.validateId(id)) {
                scannedId = utility.formatRawId(id)
            } else {
                showErrorPopup(false)
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

    function showErrorPopup(exists) {
        pageManager.enterPopup(Enums.Popup.Information, {
                                                        "text" : exists ? Strings.idAlreadyUsed : Strings.incorrectId,
                                                        "rejectButtonText": Strings.tryAgain,
                                                        "rejectButtonType": Enums.PopupAction.TryAgain,
                               }, true)
    }

    property string scannedId: ""

    property int scannedIdAction: Number(Enums.PlaceAction.Arrived)

    onScannedIdChanged: {
        if (scannedId.length > 0) {
            sessionManager.getEntityAction(scannedId, userManager.userType)
        }
    }

    property bool error: false

    Connections
    {
        target: Qt.application

        onStateChanged: {
            if (pageManager.isOnTop(page) && Qt.application.state !== Qt.ApplicationActive) {
                if (scanInput.visible) {
                    pageManager.back()
                } else {
                    retry()
                }
            }
        }
    }

    Connections
    {
        target: sessionManager
        onEntityActionDownloaded: {
            if (id == scannedId) {
                // If it arrived then user is scanning departing part.
                // If invalid then it just arrived to new location.
                if (action == Enums.PlaceAction.Arrived) {
                    scannedIdAction = Enums.PlaceAction.Departed
                } else {
                    scannedIdAction = Enums.PlaceAction.Arrived
                }

            }
        }

        onEntityActionDownloadedError: {
            if (id == scannedId) {
                showErrorPopup(exists)
            }
        }
    }

    Connections
    {
        target: pageManager
        enabled: pageManager.isOnTop(page)
        onStackViewPop: camera.stop()
        onPopupAction: {
            switch(action) {
            case Enums.PopupAction.TryAgain:
                retry()
                break
            default:
            }
        }
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
                captureMode: Camera.CaptureVideo
                focus {
                    focusMode: CameraFocus.FocusContinuous
                    focusPointMode: CameraFocus.FocusPointAuto
                }
            }

            QZXingFilter {
                id: zxingFilter
                property real normalizedScanSize: Style.normalizedScanSize
                property real normalizedScanPos: (1.0 - normalizedSize) * 0.5 //Position of scanning area is centered
                captureRect: {
                    // setup bindings
                    videoOutput.contentRect
                    videoOutput.sourceRect

                    // only scan the central quarter of the area for a barcode
                    return videoOutput.mapRectToSource(
                                videoOutput.mapNormalizedRectToItem(
                                    Qt.rect(normalizedScanPos, normalizedScanPos, normalizedScanSize, normalizedScanSize)))
                }

                decoder {
                    enabledDecoders: QZXing.DecoderFormat_QR_CODE
                    onTagFound: parseScannedId(tag)
                }
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
                width: Math.min(videoOutput.width, videoOutput.height) * zxingFilter.normalizedScanSize
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

                        additionalInputMethodHints: Qt.ImhLowercaseOnly
                        input.maximumLength: utility.getScannedIdLength()
                        input.validator : RegExpValidator { regExp : /[a-zA-Z0-9]+/ }

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
