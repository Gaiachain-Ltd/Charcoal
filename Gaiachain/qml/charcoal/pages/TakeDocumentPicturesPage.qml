import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11
import QtMultimedia 5.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.static 1.0

import "../../common" as Common
import "../../components" as Components
import "../components" as CharcoalComponents
import "../../headers" as Headers
import "../../pages" as Pages
import "../../popups" as Popups
import "../popups" as CharcoalPopups
import "../../items" as Items
import "../items" as CharcoalItems

Pages.GPage {
    enum Step {
        None,
        DocumentsInfo,
        Documents,
        DocumentsConfirm,
        DocumentsSummary,
        ReceiptsInfo,
        Receipts,
        ReceiptsConfirm,
        ReceiptsSummary
    }

    property string infoText: {
        switch (currentStatus) {
        case TakeDocumentPicturesPage.DocumentsInfo:
            return Strings.takePhotoDocumentsInfoText
        case TakeDocumentPicturesPage.ReceiptsInfo:
            return Strings.takePhotoReceiptInfoText
        default:
            return Strings.empty
        }
    }

    property string headerText: {
        switch (currentStatus) {
        case TakeDocumentPicturesPage.Documents:
            return Strings.takeAPhotoDocument
        case TakeDocumentPicturesPage.Receipts:
            return Strings.takeAPhotoReceipt
        case TakeDocumentPicturesPage.DocumentsConfirm:
        case TakeDocumentPicturesPage.ReceiptsConfirm:
            return Strings.photoTaken
        case TakeDocumentPicturesPage.DocumentsSummary:
            if (photoAccepted) {
                return Strings.photoAddedDocument
            } else {
                return Strings.photoDiscarded
            }
        case TakeDocumentPicturesPage.ReceiptsSummary:
            if (photoAccepted) {
                return Strings.photoAddedReceipt
            } else {
                return Strings.photoDiscarded
            }
        default:
            return Strings.empty
        }
    }

    property string valueText: {
        switch (currentStatus) {
        case TakeDocumentPicturesPage.DocumentsConfirm:
            return Strings.keepThePhoto
        case TakeDocumentPicturesPage.ReceiptsConfirm:
            return Strings.keepTheReceipt
        case TakeDocumentPicturesPage.DocumentsSummary:
            return Strings.addAnotherDocument
        case TakeDocumentPicturesPage.ReceiptsSummary:
            return Strings.addAnotherReceipt
        default:
            return Strings.empty
        }
    }

    property bool photoAccepted: true

    property var infoImages: [ GStyle.iconPhotoCameraGreenUrl ]

    property int currentStatus: TakeDocumentPicturesPage.DocumentsInfo
    property int backToPage: Enums.Page.InvalidPage

    backgroundColor: GStyle.backgroundShadowColor

    id: root

    title: Strings.transportReceptionDate

    logoVisible: false
    showCloseButton: false

    function backHandler() {
        closePage()
    }

    function closePage() {
        pageManager.backTo(backToPage, {
                               "documents": picturesManager.documents(),
                               "receipts": picturesManager.receipts()
                           })
        return false
    }

    Component.onCompleted: {
        if (typeof(AndroidPermissionsHandler) !== "undefined") {
            AndroidPermissionsHandler.requestPermission(AndroidPermissionsHandler.Camera)
        }
    }

    // Camera
    Camera {
        id: camera

        cameraState: Camera.ActiveState
        captureMode: Camera.CaptureStillImage

            focus {
                focusMode: CameraFocus.FocusContinuous
                focusPointMode: CameraFocus.FocusPointAuto
            }

        onErrorStringChanged: showError(errorString)

        imageCapture {
            onCaptureFailed: showError(message)

            onImageCaptured: {
                console.log("Saving to path:", picturesManager.pictureStoragePath)
                photoPreview.source = preview
            }

            onImageSaved: {
                photoPreview.savedPath = path

                if (currentStatus === TakeDocumentPicturesPage.Documents) {
                    currentStatus = TakeDocumentPicturesPage.DocumentsConfirm
                } else if (currentStatus === TakeDocumentPicturesPage.Receipts) {
                    currentStatus = TakeDocumentPicturesPage.ReceiptsConfirm
                }
            }

            onErrorStringChanged: showError(errorString)
        }
    }

    ColumnLayout {
        id: column

        anchors.fill: parent

        spacing: 0

        Rectangle {
            Layout.fillWidth: true

            color: GStyle.skipColor
            implicitHeight: 80

            visible: (currentStatus === TakeDocumentPicturesPage.Documents
                      || currentStatus === TakeDocumentPicturesPage.Receipts)

            Row {
                anchors.centerIn: parent
                spacing: s(GStyle.bigMargin)

                Items.GText {
                    id: skipText
                    font {
                        pixelSize: s(GStyle.bigPixelSize)
                        capitalization: Font.AllUppercase
                        letterSpacing: 5
                    }

                    text: Strings.skip
                    color: GStyle.textSecondaryColor
                }

                Image {
                    id: name

                    source: GStyle.skipUrl

                    height: skipText.contentHeight
                    fillMode: Image.PreserveAspectFit
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (currentStatus === TakeDocumentPicturesPage.Documents) {
                        currentStatus = TakeDocumentPicturesPage.ReceiptsInfo
                    } else if (currentStatus === TakeDocumentPicturesPage.Receipts) {
                        backHandler()
                    }
                }
            }
        }

        VideoOutput {
            id: videoOutput

            Layout.fillWidth: true
            Layout.fillHeight: true

            source: camera
            fillMode: VideoOutput.PreserveAspectCrop
            autoOrientation: true

            Rectangle {
                anchors.fill: parent
                color: "black"

                visible: (currentStatus === TakeDocumentPicturesPage.DocumentsConfirm
                          || currentStatus === TakeDocumentPicturesPage.ReceiptsConfirm
                          || (currentStatus === TakeDocumentPicturesPage.DocumentsSummary && photoAccepted)
                          || (currentStatus === TakeDocumentPicturesPage.ReceiptsSummary && photoAccepted))

                Image {
                    property string savedPath

                    id: photoPreview

                    anchors.fill: parent

                    fillMode: Image.PreserveAspectFit
                }
            }
        }

        // Buttons & status
        Rectangle {
            id: statusRect

            Layout.fillWidth: true
            color: GStyle.backgroundColor

            visible: currentStatus !== TakeDocumentPicturesPage.None
                     && currentStatus !== TakeDocumentPicturesPage.DocumentsInfo
                     && currentStatus !== TakeDocumentPicturesPage.ReceiptsInfo

            Behavior on color {
                ColorAnimation {
                    duration: GStyle.animationDuration
                }
            }

            height: s(350)

            Row {
                anchors.verticalCenter: parent.top
                anchors.horizontalCenter: parent.horizontalCenter

                spacing: 40

                CharcoalItems.CharcoalRoundButton {
                    icon.source: GStyle.iconPhotoCameraUrl
                    visible: (currentStatus === TakeDocumentPicturesPage.Documents
                              || currentStatus === TakeDocumentPicturesPage.Receipts)
                    onClicked: camera.imageCapture.captureToLocation(
                                   picturesManager.pictureStoragePath)
                }

                CharcoalItems.CharcoalRoundButton {
                    icon.source: GStyle.iconScanAnotherGreenUrl

                    visible: (currentStatus === TakeDocumentPicturesPage.DocumentsSummary
                              || currentStatus === TakeDocumentPicturesPage.ReceiptsSummary)

                    onClicked: {
                        if (currentStatus === TakeDocumentPicturesPage.DocumentsSummary) {
                            currentStatus = TakeDocumentPicturesPage.Documents
                        } else if (currentStatus === TakeDocumentPicturesPage.ReceiptsSummary) {
                            currentStatus = TakeDocumentPicturesPage.Receipts
                        }
                    }
                }

                CharcoalItems.CharcoalRoundButton {
                    icon.source: GStyle.logoutImgUrl

                    visible: (currentStatus === TakeDocumentPicturesPage.DocumentsSummary
                              || currentStatus === TakeDocumentPicturesPage.ReceiptsSummary)

                    onClicked: {
                        if (currentStatus === TakeDocumentPicturesPage.DocumentsSummary) {
                            currentStatus = TakeDocumentPicturesPage.ReceiptsInfo
                        } else if (currentStatus === TakeDocumentPicturesPage.ReceiptsSummary) {
                            backHandler()
                        }
                    }
                }

                CharcoalItems.CharcoalRoundButton {
                    text: Strings.yes
                    icon.source: GStyle.iconYesUrl
                    display: AbstractButton.TextBesideIcon
                    width: 2 * height
                    icon.height: s(20)
                    icon.width: icon.height
                    palette.buttonText: GStyle.okColor

                    visible: (currentStatus === TakeDocumentPicturesPage.DocumentsConfirm
                              || currentStatus === TakeDocumentPicturesPage.ReceiptsConfirm)

                    onClicked: {
                        photoAccepted = true
                        if (currentStatus === TakeDocumentPicturesPage.DocumentsConfirm) {
                            picturesManager.saveDocumentPhoto(photoPreview.savedPath)
                            currentStatus = TakeDocumentPicturesPage.DocumentsSummary
                        } else if (currentStatus === TakeDocumentPicturesPage.ReceiptsConfirm) {
                            picturesManager.saveReceiptPhoto(photoPreview.savedPath)
                            currentStatus = TakeDocumentPicturesPage.ReceiptsSummary
                        }
                    }
                }

                CharcoalItems.CharcoalRoundButton {
                    text: Strings.no
                    icon.source: GStyle.iconNoUrl
                    display: AbstractButton.TextBesideIcon
                    width: 2 * height
                    icon.height: s(20)
                    icon.width: icon.height
                    palette.buttonText: GStyle.errorColor

                    visible: (currentStatus === TakeDocumentPicturesPage.DocumentsConfirm
                              || currentStatus === TakeDocumentPicturesPage.ReceiptsConfirm)

                    onClicked: {
                        photoAccepted = false
                        picturesManager.discardPhoto(photoPreview.savedPath)

                        if (currentStatus === TakeDocumentPicturesPage.DocumentsConfirm) {
                            currentStatus = TakeDocumentPicturesPage.DocumentsSummary
                        } else if (currentStatus === TakeDocumentPicturesPage.ReceiptsConfirm) {
                            currentStatus = TakeDocumentPicturesPage.ReceiptsSummary
                        }
                    }
                }
            }

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                property color textColor: GStyle.textPrimaryColor

                Item {
                    Layout.fillHeight: true
                    width: 1
                }

                Items.GText {
                    Layout.fillWidth: true
                    text: headerText
                    elide: Text.ElideNone
                    wrapMode: Text.WordWrap
                    color: parent.textColor
                    font.capitalization: Font.AllUppercase
                    font.pixelSize: s(GStyle.pixelSize)
                    verticalAlignment: Text.AlignBottom
                }

                Items.GText {
                    Layout.fillWidth: true
                    text: valueText
                    elide: Text.ElideNone
                    wrapMode: Text.WordWrap
                    color: parent.textColor
                    visible: text.length > 0
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

    CharcoalComponents.InfoOverlay {
        id: infoOverlay
        anchors.fill: parent

        visible: (currentStatus === TakeDocumentPicturesPage.DocumentsInfo
                  || currentStatus === TakeDocumentPicturesPage.ReceiptsInfo)
        images: infoImages
        text: infoText

        onHidden: {
            if (currentStatus === TakeDocumentPicturesPage.DocumentsInfo) {
                currentStatus = TakeDocumentPicturesPage.Documents
            } else if (currentStatus === TakeDocumentPicturesPage.ReceiptsInfo) {
                currentStatus = TakeDocumentPicturesPage.Receipts
            }
        }
    }

    function showError(text) {
        notificationPopup.text = Strings.cameraErrorString.arg(text)
        notificationPopup.open()
    }

    Popups.NotificationPopup {
        id: notificationPopup
        backgroundColor: GStyle.errorColor
    }
}
