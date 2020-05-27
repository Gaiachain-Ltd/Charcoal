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
        case TakeDocumentPicturesPage.ReceiptInfo:
            return Strings.takePhotoReceiptInfoText
        default:
            return Strings.empty
        }
    }

    property string headerText: {
        switch (currentStatus) {
        case TakeDocumentPicturesPage.Documents:
        case TakeDocumentPicturesPage.Receipts:
            return Strings.takeAPhoto
        case TakeDocumentPicturesPage.DocumentsConfirm:
        case TakeDocumentPicturesPage.ReceiptsConfirm:
            return Strings.photoTaken
        case TakeDocumentPicturesPage.DocumentsSummary:
        case TakeDocumentPicturesPage.ReceiptsSummary:
            return Strings.photoAdded
        default:
            return Strings.empty
        }
    }

    property string valueText: {
        switch (currentStatus) {
        case TakeDocumentPicturesPage.DocumentsConfirm:
        case TakeDocumentPicturesPage.ReceiptsConfirm:
            return Strings.uploadThis
        case TakeDocumentPicturesPage.DocumentsSummary:
        case TakeDocumentPicturesPage.ReceiptsSummary:
            return Strings.addAnother
        default:
            return Strings.empty
        }
    }

    property var infoImages: [ GStyle.iconPhotoCameraGreenUrl ]

    property var documentPhotos: []
    property var receiptPhotos: []

    property int currentStatus: TakeDocumentPicturesPage.DocumentsInfo

    onCurrentStatusChanged: console.log("Status:", currentStatus)

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
                               "documents": documentPhotos,
                               "receipts": receiptPhotos
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

        cameraState: (currentStatus === TakeDocumentPicturesPage.Documents
                      || currentStatus === TakeDocumentPicturesPage.Receipts)?
                         Camera.ActiveState : Camera.LoadedState

        captureMode: Camera.CaptureStillImage

        focus {
            focusMode: CameraFocus.FocusContinuous
            focusPointMode: CameraFocus.FocusPointAuto
        }

        onErrorStringChanged: console.log("Error!", errorCode, errorString)

        imageCapture {
            onCaptureFailed: console.log("Capture failed!") // TODO: toast message!
            onImageCaptured: {
                photoPreview.source = preview
                console.log("Image captured", preview)

                if (currentStatus === TakeDocumentPicturesPage.Documents) {
                    currentStatus = TakeDocumentPicturesPage.DocumentsConfirm
                } else if (currentStatus === TakeDocumentPicturesPage.Receipts) {
                    currentStatus = TakeDocumentPicturesPage.ReceiptsConfirm
                }
            }

            onErrorStringChanged: console.log("Capture error!", errorString)
        }
    }

    ColumnLayout {
        id: column

        anchors.fill: parent

        spacing: 0

        VideoOutput {
            id: videoOutput

            Layout.fillWidth: true
            Layout.fillHeight: true

            source: camera
            fillMode: VideoOutput.PreserveAspectCrop
            autoOrientation: true

//            Image {
//                source: GStyle.loginBackgroundOriginalUrl
//                anchors {
//                    left: parent.left
//                    right: parent.right
//                }

//                fillMode: Image.Tile
//                visible: camera.cameraState !== Camera.ActiveState
//            }

            Image {
                id: photoPreview

                anchors.fill: parent

                fillMode: Image.PreserveAspectFit
                visible: (currentStatus === TakeDocumentPicturesPage.DocumentsConfirm
                          || currentStatus === TakeDocumentPicturesPage.ReceiptsConfirm
                          || currentStatus === TakeDocumentPicturesPage.DocumentsSummary
                          || currentStatus === TakeDocumentPicturesPage.ReceiptsSummary)
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
                    onClicked: { camera.start(); console.log("Camera started") }
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
                    icon.source: GStyle.iconYesUrl
                    visible: (currentStatus === TakeDocumentPicturesPage.DocumentsConfirm
                              || currentStatus === TakeDocumentPicturesPage.ReceiptsConfirm)

                    onClicked: {
                        if (currentStatus === TakeDocumentPicturesPage.DocumentsConfirm) {
                            // TODO: add photo
                            currentStatus = TakeDocumentPicturesPage.DocumentsSummary
                        } else if (currentStatus === TakeDocumentPicturesPage.ReceiptsConfirm) {
                            // TODO: add photo
                            currentStatus = TakeDocumentPicturesPage.ReceiptsSummary
                        }
                    }
                }

                CharcoalItems.CharcoalRoundButton {
                    icon.source: GStyle.iconNoUrl
                    visible: (currentStatus === TakeDocumentPicturesPage.DocumentsConfirm
                              || currentStatus === TakeDocumentPicturesPage.ReceiptsConfirm)

                    onClicked: {
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
}
