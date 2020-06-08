import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.types 1.0

import "../../items" as Items
import "../../common" as Common
import "../../headers" as Headers
import "../headers" as CharcoalHeaders
import "../../pages" as Pages

Pages.SupplyChainPageBase {
    id: top

    property var scannedQrs: [
        "2222-2222-2222",
        "3333-3333-3333",
        "LH4U-3YJT-LFND"
    ]

    onScannedQrsChanged: console.log("Scanned IDs:", prepareScannedIds)

    property var documents: []
    property var receipts: []

    function prepareScannedIds() {
        let result = []
        for (let qr of scannedQrs) {
            result.push([harvestIdComboBox.currentText + "/B"
                         + Utility.constDigitsNumber(
                             scannedQrs.indexOf(qr), 3), qr])
        }
        return result;
    }

    title: Strings.reception

    Component.onCompleted: refreshData()

    function refreshData() {
        //sessionManager.getUnusedLotIds()
    }

    function proceed() {
        pageManager.enter(Enums.Page.SupplyChainSummary, {
                              "supplyChainPage": this,
                              "summary": summary(),
                              "proceedButtonText": Strings.proceed
                          });
    }

    function summary() {
        let docs = picturesManager.documents()
        let recs = picturesManager.receipts()
        let hasDocs = docs.length > 0
        let hasRecs = recs.length > 0
        let docsIcon = "image://tickmark/document-" + hasDocs
        let recsIcon = "image://tickmark/receipt-" + hasRecs

        // TODO: guess transport ID based on bags codes!
        var summary = [
                    createSummaryItem(Strings.transportId,
                                      // TODO: insert real data!
                                      "AM003PM/0595112/04-03-2020/AM004NA/1234AB56/31-03-2020",
                                      "", "",
                                      Pages.SupplyChainPageBase.Standard,
                                      GStyle.delegateHighlightColor4,
                                      GStyle.fontHighlightColor4),
                    createSummaryItem("",
                                      [
                                          [
                                              Strings.numberOfBags,
                                              Strings.documents,
                                              Strings.receipt
                                          ],
                                          [
                                              Strings.numberOfBagsDetail.arg("149").arg("150"),
                                              hasDocs? Strings.approved : Strings.noPhoto,
                                              hasRecs? Strings.approved : Strings.noPhoto
                                          ],
                                          [
                                              GStyle.warningUrl,
                                              docsIcon,
                                              recsIcon
                                          ],
                                          [
                                              "",
                                              [
                                                  hasDocs?
                                                      Enums.Page.PhotoGallery
                                                    : Enums.Page.InvalidPage,
                                                  [
                                                      "urls", docs
                                                  ]
                                              ],
                                              [
                                                  hasRecs?
                                                      Enums.Page.PhotoGallery
                                                    : Enums.Page.InvalidPage,
                                                  [
                                                      "urls", recs
                                                  ]
                                              ]
                                          ]
                                      ],
                                      "", "",
                                      Pages.SupplyChainPageBase.ColumnStack,
                                      GStyle.delegateHighlightColor4,
                                      GStyle.fontHighlightColor4,
                                      GStyle.textPrimaryColor),
                    createSummaryItem(Strings.plateNumber,
                                      // TODO: insert real data!
                                      "12345AA"),
                    createSummaryItem(Strings.receptionDateCharcoal,
                                      unloadingDateHeader.selectedDate.toLocaleDateString(
                                          Qt.locale(), Strings.dateFormat)),
                    createSummaryItem(Strings.gpsCoordinates, gpsSource.coordinate.toString())
                ]
        return summary
    }

    function addAction() {
        /*
        showOverlay()

        var properties = {
            [PackageDataProperties.LotPid]: 1,
            [PackageDataProperties.HarvestWeights]: 1
        }

        // ID, action, coordiate, timestamp, props
        dataManager.addAction(
                    plotId,
                    Enums.SupplyChainAction.LoggingBeginning,
                    (gpsSource.coordinate? gpsSource.coordinate
                                         : QtPositioning.coordinate()),
                    new Date,
                    properties)
                    */
        console.warn("Dummy action - TODO implement! Going back to main menu")
        pageManager.enter(Enums.Page.MainMenu)
    }

    Headers.ButtonInputHeader {
        id: receiveDocsHeader

        Layout.fillWidth: true
        readOnly: true
        forceBoldValue: true
        valueFontSize: s(GStyle.titlePixelSize)
        inputText: Strings.receiveDocumentsAndReceipt
        onClicked: pageManager.enter(Enums.Page.TakeDocumentPictures,
                                     {
                                         "backToPage": Enums.Page.SupplyChainReception
                                     })
    }

    Headers.ButtonInputHeader {
        id: scanAllBagsHeader

        Layout.fillWidth: true
        readOnly: true
        forceBoldValue: true
        valueFontSize: s(GStyle.titlePixelSize)
        inputText: Strings.scanAllBagsFromTruck
        onClicked: pageManager.enter(Enums.Page.QRScanner,
                                     {
                                         "title": top.title,
                                         "infoText": Strings.scanAllBagsToCheckInfoText,
                                         "backToPage": Enums.Page.SupplyChainReception,
                                         "infoImages": [ GStyle.bagsReceptionUrl ],
                                         "idBase": "AM003PM/0595112/04-03-2020/AM004NA",
                                         "scannedQrs": scannedQrs
                                     })
    }

    CharcoalHeaders.CharcoalInputDateHeader {
        id: unloadingDateHeader
        Layout.fillWidth: true
        headerText: Strings.receptionDateCharcoal
        helpButtonVisible: true
        helpText: Strings.receptionUnloadingDateHelp
    }

    Common.PositionSourceHandler {
        id: gpsSource

        function errorMessage() {
            if (noAccess) {
                return Strings.enableGpsLocation
            } else if (!valid) {
                return Strings.gpsNotAvailable
            } else if (!positioningSupported) {
                return Strings.gpsTurnedOff
            } else {
                return Strings.gpsInvalid
            }
        }
    }

    Headers.ButtonInputHeader {
        id: gpsCoordinatesButtonInputHeader
        helpButtonVisible: true
        helpText: Strings.receptionGpsHelp

        Layout.fillWidth: true

        headerText: Strings.gpsCoordinates
        inputText: (gpsSource.validCoordinate ? Helper.formatCoordinate(gpsSource.coordinate.toString()) : gpsSource.errorMessage())
        iconSource: (gpsSource.validCoordinate ? GStyle.gpsOkImgUrl : GStyle.gpsFailedImgUrl)

        onClicked: gpsSource.update()
    }
}
