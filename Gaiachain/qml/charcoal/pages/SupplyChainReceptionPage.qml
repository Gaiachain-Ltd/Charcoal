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
                                              Strings.approved,
                                              Strings.noPhoto
                                          ],
                                          [
                                              GStyle.warningUrl,
                                              GStyle.uploadOkUrl,
                                              GStyle.noPhotoUrl
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
                                         "infoText1": GStyle.takePhotoDocumentsInfoText,
                                         "infoText2": GStyle.takePhotoReceiptInfoText
                                     })
    }

    Headers.ButtonInputHeader {
        id: scanAllBagsHeader

        Layout.fillWidth: true
        readOnly: true
        forceBoldValue: true
        valueFontSize: s(GStyle.titlePixelSize)
        inputText: Strings.scanAllBagsFromTruck
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
