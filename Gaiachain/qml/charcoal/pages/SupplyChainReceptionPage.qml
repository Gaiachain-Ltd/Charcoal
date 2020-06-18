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

    property string transportId

    property var scannedQrs: [
        "2222-2222-2222",
        "3333-3333-3333",
        "LH4U-3YJT-LFND"
    ]

    property var documents: []
    property var receipts: []

    title: Strings.reception

    Component.onCompleted: refreshData()

    function refreshData() {
        // Nothing to do
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

        transportId = dataManager.actionController.getTransportIdFromBags(scannedQrs)
        let bagCount = dataManager.actionController.bagCountInTransport(transportId)
        // TODO: check if all QRs are MATCHING!
        let allBags = scannedQrs.length === bagCount

        var summary = [
                    createSummaryItem(Strings.transportId,
                                      transportId,
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
                                              Strings.numberOfBagsDetail
                                                .arg(scannedQrs.length)
                                                .arg(bagCount),
                                              hasDocs? Strings.approved : Strings.noPhoto,
                                              hasRecs? Strings.approved : Strings.noPhoto
                                          ],
                                          [
                                              allBags? GStyle.checkGreenUrl : GStyle.warningUrl,
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
                                      dataManager.actionController.plateNumberInTransport(transportId)),
                    createSummaryItem(Strings.receptionDateCharcoal,
                                      unloadingDateHeader.selectedDate.toLocaleDateString(
                                          Qt.locale(), Strings.dateFormat)),
                    createSummaryItem(Strings.gpsCoordinates, gpsSource.coordinate.toString())
                ]
        return summary
    }

    function addAction() {
        dataManager.actionController.registerReception(
                    (gpsSource.coordinate? gpsSource.coordinate
                                         : QtPositioning.coordinate()),
                    unloadingDateHeader.selectedDate,
                    userManager.userData.code,
                    transportId,
                    picturesManager.documents(),
                    picturesManager.receipts(),
                    scannedQrs
                    )

        let plotId = dataManager.actionController.getPlotId(transportId)
        let scannedBagsCount = dataManager.actionController.scannedBagsCount(transportId)
        let scannedBagsTotal = dataManager.actionController.scannedBagsTotal(transportId)
        let registeredTrucksCount = dataManager.actionController.registeredTrucksCount(transportId)
        let registeredTrucksTotal = dataManager.actionController.registeredTrucksTotal(transportId)

        pageManager.enter(Enums.Page.SupplyChainFinalize, {
                              "plotId": plotId,
                              "scannedBagsCount": scannedBagsCount,
                              "scannedBagsTotal": scannedBagsTotal,
                              "registeredTrucksCount": registeredTrucksCount,
                              "registeredTrucksTotal": registeredTrucksTotal
                          })
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
                                         // TODO: use proper harvest ID here!
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
