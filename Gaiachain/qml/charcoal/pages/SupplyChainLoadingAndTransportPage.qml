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

    onScannedQrsChanged: console.log("Scanned IDs:", prepareScannedIds)

    function prepareScannedIds() {
        let result = []
        for (let qr of scannedQrs) {
            result.push([harvestIdComboBox.currentText + "/B"
                         + Utility.constDigitsNumber(
                             scannedQrs.indexOf(qr), 3), qr])
        }
        return result;
    }

    title: Strings.loadingAndTransport

    Component.onCompleted: refreshData()

    function refreshData() {
        dataManager.nusedHarvestIdsModel.refresh()
        dataManager.destinationsModel.refresh()
    }

    function proceed() {
        pageManager.enter(Enums.Page.SupplyChainSummary, {
                              "supplyChainPage": this,
                              "summary": summary(),
                              "proceedButtonText": Strings.proceed
                          });
    }

    function summary() {
        transportId = dataManager.actionController.generateTransportId(
                        harvestIdComboBox.currentText,
                        plateNumberHeader.inputText,
                        dataManager.actionController.nextTransportNumber(
                            harvestIdComboBox.currentText),
                        loadingDateHeader.selectedDate
                    )
        var summary = [
                    createSummaryItem(Strings.transportId,
                                      transportId,
                                      "", "",
                                      Pages.SupplyChainPageBase.Standard,
                                      GStyle.delegateHighlightColor4,
                                      GStyle.fontHighlightColor4),
                    createSummaryItem(Strings.numberOfBags,
                                      scannedQrs.length),
                    createSummaryItem(Strings.harvestId,
                                      harvestIdComboBox.currentText,
                                      "", "",
                                      Pages.SupplyChainPageBase.Standard,
                                      GStyle.delegateHighlightColor2,
                                      GStyle.fontHighlightColor2),
                    createSummaryItem(Strings.plateNumber,
                                      plateNumberHeader.inputText),
                    createSummaryItem(Strings.loadingDate,
                                      loadingDateHeader.selectedDate.toLocaleDateString(
                                          Qt.locale(), Strings.dateFormat)),
                    createSummaryItem(Strings.deliveryDestination,
                                      deliveryDestinationComboBox.currentText),
                    createSummaryItem(Strings.gpsCoordinates, gpsSource.coordinate.toString())
                ]
        return summary
    }

    function addAction() {
        dataManager.actionController.registerTransportAndLoading(
                    (gpsSource.coordinate? gpsSource.coordinate
                                         : QtPositioning.coordinate()),
                    loadingDateHeader.selectedDate,
                    userManager.userData.code,
                    transportId,
                    harvestIdComboBox.currentText,
                    plateNumberHeader.inputText,
                    deliveryDestinationComboBox.currentText,
                    scannedQrs
                    )

        pageManager.enter(Enums.Page.MainMenu)
    }

    CharcoalHeaders.CharcoalComboBoxHeader {
        id: harvestIdComboBox
        Layout.fillWidth: true
        headerText: Strings.harvestId
        helpButtonVisible: true
        helpText: Strings.loadingAndTransportHarvestIdHelp

        checkIcon: GStyle.checkGreenUrl
        delegateTextColor: GStyle.fontHighlightColor2

        model: dataManager.unusedHarvestIdsModel
    }

    Headers.InputHeader {
        id: plateNumberHeader
        Layout.fillWidth: true
        headerText: Strings.plateNumber
        helpButtonVisible: true
        helpText: Strings.loadingAndTransportPlateNumberHelp
        inputText: "1234AM56"
    }

    Headers.ButtonInputHeader {
        id: loadAndScanBagsHeader

        Layout.fillWidth: true
        readOnly: true
        forceBoldValue: true
        valueFontSize: s(GStyle.titlePixelSize)
        inputText: Strings.loadAndScanBags
        onClicked: pageManager.enter(Enums.Page.QRScanner,
                                     {
                                         "title": top.title,
                                         "infoText": Strings.scanAllBagsInfoText,
                                         "backToPage":
                                         Enums.Page.SupplyChainLoadingAndTransport,
                                         "infoImages": [ GStyle.bagsLoadingUrl ],
                                         "showProceedPage": true,
                                         "truckId": plateNumberHeader.inputText,
                                         "idBase": harvestIdComboBox.currentText,
                                         "scannedQrs": scannedQrs
                                     })
    }

    CharcoalHeaders.CharcoalInputDateHeader {
        id: loadingDateHeader
        Layout.fillWidth: true
        headerText: Strings.loadingDate
        helpButtonVisible: true
        helpText: Strings.loadingAndTransportLoadingDateHelp
    }

    CharcoalHeaders.CharcoalComboBoxHeader {
        id: deliveryDestinationComboBox
        Layout.fillWidth: true
        headerText: Strings.deliveryDestination
        helpButtonVisible: true
        helpText: Strings.loadingAndTransportDeliveryDestinationHelp

        model: dataManager.destinationsModel
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
        helpText: Strings.loadingAndTransportGpsHelp

        Layout.fillWidth: true

        headerText: Strings.gpsCoordinates
        inputText: (gpsSource.validCoordinate ? Helper.formatCoordinate(gpsSource.coordinate.toString()) : gpsSource.errorMessage())
        iconSource: (gpsSource.validCoordinate ? GStyle.gpsOkImgUrl : GStyle.gpsFailedImgUrl)

        onClicked: gpsSource.update()
    }
}



