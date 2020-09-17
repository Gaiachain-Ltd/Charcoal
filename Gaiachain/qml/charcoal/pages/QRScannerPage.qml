import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11
import QtMultimedia 5.12
import QZXing 2.3

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

    property string infoText: "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec sagittis non nibh quis aliquam. Praesent vitae tempus velit. Donec id sem finibus lacus blandit tempus ac commodo elit. Donec sagittis consectetur nisl non eleifend."
    property var infoImages: [ GStyle.iconPhotoCameraGreenUrl ]

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

    onStatusTextHeaderChanged: console.log("Header", statusTextHeader)

    property string statusTextValue: currentQr

    property int currentStatus: QRScannerPage.None

    property int backToPage: Enums.Page.InvalidPage

    property bool showProceedPage: false
    property string truckId
    property var scannedQrs

    onScannedQrsChanged: console.log("Scanned IDs:", scannedQrs)

    property string currentQr: ""
    property bool shouldPause: false

    backgroundColor: GStyle.backgroundShadowColor

    id: root

    title: Strings.qrCode

    logoVisible: false
    showCloseButton: false

    function backHandler() {
        scannedBagsPopup.open()
    }

    function closePage() {
        pageManager.backTo(backToPage, {
                               "scannedQrs": prepareScannedIds(),
                               "shouldPause": shouldPause
                           })
        return false
    }

    function prepareScannedIds() {
        return scannedQrs;
    }

    Component.onCompleted: {
        if (typeof(AndroidPermissionsHandler) !== "undefined") {
            AndroidPermissionsHandler.requestPermission(AndroidPermissionsHandler.Camera)
        }
    }

    function parseScannedId(id) {
        if (Utility.validateId(id)) {
            currentQr = Utility.formatRawId(id)

            // Check for ID duplicates:
            for (let scanned of scannedQrs) {
                if (scanned === currentQr) {
                    console.log("Code duplicated!", currentQr)
                    currentStatus = QRScannerPage.Warning
                    return
                }
            }

            let temp = scannedQrs
            temp.push(currentQr)
            scannedQrs = temp
            currentStatus = QRScannerPage.Success
        } else {
            currentQr = id
            currentStatus = QRScannerPage.Error
            console.warn("Wrong code content", id)
        }
    }

    // Camera
    Camera {
        id: camera

        cameraState: (currentStatus === QRScannerPage.Scanning)? Camera.ActiveState
                                                               : Camera.LoadedState

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
                console.log("Barcode data: " + tag)
                if (currentStatus === QRScannerPage.Scanning) {
                    parseScannedId(tag)
                }
            }

            tryHarder: true
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
            filters: [zxingFilter]
            fillMode: VideoOutput.PreserveAspectCrop
            autoOrientation: true

            Image {
                source: GStyle.loginBackgroundOriginalUrl
                anchors {
                    left: parent.left
                    right: parent.right
                }

                fillMode: Image.Tile
                visible: camera.cameraState !== Camera.ActiveState
            }

            Image {
                width: zxingFilter.normalizedScanSize * videoOutput.width
                height: zxingFilter.normalizedScanSize * videoOutput.height

                source: GStyle.frameImgUrl
                anchors.centerIn: parent
                visible: camera.cameraState === Camera.ActiveState
            }

            Rectangle {
                id: proceedOverlay
                anchors.fill: parent
                color: GStyle.backgroundShadowColor
                visible: currentStatus === QRScannerPage.Proceed

                Row {
                    anchors.centerIn: parent
                    spacing: 2 * s(GStyle.bigMargin)

                    Repeater {
                        property alias truckId: root.truckId
                        property int qrCount: scannedQrs.length

                        id: repeater
                        model: [ GStyle.truckUrl, GStyle.bagUrl ]

                        ColumnLayout {
                            spacing: 2 * s(GStyle.bigMargin)

                            Image {
                                Layout.alignment: Qt.AlignHCenter
                                source: modelData
                            }

                            Items.GText {
                                Layout.alignment: Qt.AlignHCenter
                                text: index === 0? repeater.truckId : repeater.qrCount
                                color: GStyle.statusGreen
                                elide: Text.ElideNone
                                wrapMode: Text.WordWrap
                                font.capitalization: Font.AllUppercase
                                font.bold: true
                                font.pixelSize: s(GStyle.bigPixelSize)
                            }
                        }
                    }
                }
            }
        }

        // Buttons & status
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

            visible: currentStatus !== QRScannerPage.None

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
                    icon.source: GStyle.iconRetryScanRedUrl
                    visible: currentStatus === QRScannerPage.Error
                    onClicked: currentStatus = QRScannerPage.Scanning
                }

                CharcoalItems.CharcoalRoundButton {
                    icon.source: currentStatus === QRScannerPage.Warning?
                                     GStyle.iconScanAnotherOrangeUrl
                                   : GStyle.iconScanAnotherGreenUrl

                    visible: (currentStatus === QRScannerPage.Warning
                              || currentStatus === QRScannerPage.Success)

                    onClicked: currentStatus = QRScannerPage.Scanning
                }

                CharcoalItems.CharcoalRoundButton {
                    icon.source: GStyle.iconListUrl

                    visible: (currentStatus === QRScannerPage.Warning
                              || currentStatus === QRScannerPage.Success)

                    onClicked: popup.open()
                }

                CharcoalItems.CharcoalRoundButton {
                    icon.source: GStyle.iconKeyboardUrl
                    icon.height: s(35)
                    icon.width: s(50)

                    visible: (currentStatus === QRScannerPage.Scanning
                              || currentStatus === QRScannerPage.Error)

                    onClicked: currentStatus = QRScannerPage.ManualScan
                }

                CharcoalItems.CharcoalRoundButton {
                    icon.source: GStyle.logoutImgUrl

                    visible: (currentStatus === QRScannerPage.Scanning
                              || currentStatus === QRScannerPage.Success
                              || currentStatus === QRScannerPage.Warning
                              || currentStatus === QRScannerPage.Error)

                    onClicked: {
                        if (showProceedPage) {
                            currentStatus = QRScannerPage.Proceed
                        } else {
                            backHandler()
                        }
                    }
                }

                CharcoalItems.CharcoalRoundButton {
                    icon.source: GStyle.iconYesUrl
                    visible: (currentStatus === QRScannerPage.Proceed
                              || currentStatus === QRScannerPage.ManualScan)

                    onClicked: {
                        if (currentStatus === QRScannerPage.Proceed) {
                            shouldPause = true
                            closePage()
                        } else if (currentStatus === QRScannerPage.ManualScan) {
                            parseScannedId(currentQr)
                        }
                    }
                }

                CharcoalItems.CharcoalRoundButton {
                    icon.source: GStyle.iconNoUrl
                    visible: (currentStatus === QRScannerPage.Proceed
                              || currentStatus === QRScannerPage.ManualScan)

                    onClicked: {
                        if (currentStatus === QRScannerPage.Proceed) {
                            closePage()
                        } else if (currentStatus === QRScannerPage.ManualScan) {
                            currentStatus = QRScannerPage.Scanning
                        }
                    }
                }
            }

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                property color textColor: (currentStatus === QRScannerPage.None
                                           || currentStatus === QRScannerPage.Scanning)?
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
                    visible: text.length > 2 && (currentStatus !== QRScannerPage.Scanning)
                    font.capitalization: Font.AllUppercase
                    font.pixelSize: s(GStyle.bigPixelSize)
                    verticalAlignment: Text.AlignTop
                }

                CharcoalItems.CharcoalQrInput {
                    Layout.leftMargin: s(GStyle.bigMargin)
                    Layout.rightMargin: s(GStyle.bigMargin)
                    Layout.fillWidth: true

                    id: manualInput

                    visible: currentStatus === QRScannerPage.ManualScan

                    onVisibleChanged: {
                      if (visible) {
                        clear()
                      }
                    }

                    onQrCodeChanged: currentQr = qrCode
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

        visible: infoVisible
        images: infoImages
        text: infoText

        onHidden: {
            currentStatus = QRScannerPage.Scanning
            infoVisible = false
        }
    }

    Popup {
        id: popup

        property bool showCloseButton: false
        property bool logoVisible: false

        property string title: root.title + " (%1)".arg(entriesList.count)

        onOpened: entriesList.idBase = dataManager.actionController.getTransportIdFromBags(
                      scannedQrs,
                      (backToPage === Enums.Page.SupplyChainLoadingAndTransport))

        anchors.centerIn: Overlay.overlay
        width: Overlay.overlay? Overlay.overlay.width : 100
        height: Overlay.overlay? Overlay.overlay.height : 100

        focus: true
        modal: true

        ColumnLayout {
            anchors.fill: parent

            spacing: 0

            Components.NavigationHeader {
                id: header
                Layout.fillWidth: true

                title: popup.title
                showCloseButton: popup.showCloseButton
                showBackButton: true
                enableBackButton: true
                logoVisible: popup.logoVisible

                // Reimplemented NavigationHeader.goBack()
                function goBack() {
                    popup.close()
                }
            }

            ListView {
                property string idBase

                id: entriesList
                Layout.fillHeight: true
                Layout.fillWidth: true

                clip: true

                boundsBehavior: Flickable.StopAtBounds

                model: root.scannedQrs

                function removeRow(index) {
                    let temp = root.scannedQrs
                    temp.splice(index, 1)
                    root.scannedQrs = temp
                }

                delegate: Item {
                    readonly property string bagId: entriesList.idBase + "/B"
                                                    + Utility.constDigitsNumber(index + 1, 3)
                    readonly property string qrCode: modelData

                    id: delegateItem
                    width: entriesList.width
                    height: layout.implicitHeight

                    ColumnLayout {
                        id: layout
                        anchors.fill: parent
                        RowLayout {
                            Layout.topMargin: s(GStyle.smallMargin)

                            ColumnLayout {
                                Items.GText {
                                    Layout.fillWidth: true
                                    text: delegateItem.bagId
                                    color: GStyle.textPrimaryColor
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignLeft
                                    font.bold: true
                                    font.pixelSize: s(GStyle.smallPixelSize)
                                    wrapMode: Text.WrapAnywhere
                                }

                                Items.GText {
                                    Layout.fillWidth: true
                                    text: delegateItem.qrCode
                                    color: GStyle.calendarBlockedFontColor
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignLeft
                                    font.bold: false
                                    font.pixelSize: s(GStyle.tinyPixelSize)
                                    font.capitalization: Font.AllUppercase
                                }
                            }

                            Image {
                                id: closeImage
                                Layout.rightMargin: s(GStyle.middleMargin)
                                Layout.topMargin: s(GStyle.tinyMargin)
                                Layout.bottomMargin: s(GStyle.tinyMargin)
                                Layout.fillHeight: true
                                Layout.minimumWidth: height
                                source: GStyle.deleteImgUrl
                                // This can make the SVGs more sharp
                                //sourceSize: Qt.size(width, height)
                                fillMode: Image.PreserveAspectFit

                                //Rectangle {
                                //    color: "#5500ff00"
                                //    anchors.fill: parent
                                //}
                            }
                        }                        

                        Rectangle {
                            Layout.fillWidth: true

                            color: GStyle.separatorColor
                            height: 1
                        }
                    }

                    MouseArea {
                        anchors {
                            top: parent.top
                            bottom: parent.bottom
                            right: parent.right
                            margins: s(GStyle.microMargin)
                        }

                        width: closeImage.width * 2

                        onClicked: entriesList.removeRow(index)

                        //Rectangle {
                        //    color: "#55ff0000"
                        //    anchors.fill: parent
                        //}
                    }
                }

                ScrollBar.vertical: ScrollBar {}
            }

            Headers.ButtonInputHeader {
                id: selectButton

                Layout.leftMargin: s(GStyle.hugeMargin)
                Layout.rightMargin: Layout.leftMargin
                Layout.bottomMargin: Layout.leftMargin
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignBottom

                enabled: true

                inputText: Strings.deleteAllScannedCodes
                iconSource: GStyle.iconDeleteAll

                onWholeInputClicked: clearBagsPopup.open()
            }

            Components.Footer {
                id: footer
                Layout.fillWidth: true
            }

        }
    }

    CharcoalPopups.ColumnButtonsPopup {
        id: clearBagsPopup

        text: Strings.clearAllScannedQrCodes
        rejectButtonText: Strings.no
        acceptButtonText: Strings.clear

        onRejected: clearBagsPopup.close()

        onAccepted: {
            root.scannedQrs = []
            clearBagsPopup.close()
        }
    }

    CharcoalPopups.ColumnButtonsPopup {
        id: scannedBagsPopup
        property bool isRestore: currentStatus === QRScannerPage.Scanning

        text: scannedBagsPopup.isRestore?
                  Strings.scannedBagsPopupText.arg(scannedQrs.length)
                : Strings.scannedBagsSavePopupText.arg(scannedQrs.length)

        rejectButtonText: scannedBagsPopup.isRestore? Strings.deleteText
                                                    : Strings.no

        acceptButtonText: scannedBagsPopup.isRestore? Strings.restore
                                                    : Strings.save

        onRejected: {
            scannedQrs = []
            closePage()
        }

        onAccepted: {
            if (scannedBagsPopup.isRestore) {
                scannedBagsPopup.close()
            } else {
                closePage()
            }
        }
    }
}
