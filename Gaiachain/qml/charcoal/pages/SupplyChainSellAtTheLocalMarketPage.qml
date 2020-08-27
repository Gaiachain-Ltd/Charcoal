import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../../items" as Items
import "../../common" as Common
import "../../headers" as Headers
import "../headers" as CharcoalHeaders
import "../../pages" as Pages

Pages.SupplyChainPageBase {
    id: top

    property string transportName
    property int transportId

    property var scannedQrs: []
    property var bagsMatch: undefined
    readonly property bool hasQrs: scannedQrs.length > 0

    onScannedQrsChanged: {
        // Note: you cannot use `hasQrs` here, because it is not reevaluated
        // at this point, yet.
        if (scannedQrs.length > 0) {
            transportId = dataManager.actionController.getTransportIdFromBags(scannedQrs)
            if (transportId !== -1) {
                transportName = dataManager.actionController.getEntityName(transportId)
                dataManager.minimumDateModel.plotId = transportId
                bagsMatch = dataManager.actionController.matchBags(
                            transportId, scannedQrs)

                if (bagsMatch.hasConflict) {
                    pageManager.openPopup(Enums.Popup.Information,
                                          { "text": Strings.duplicatedBagError,
                                              "buttonPrimaryColor": GStyle.errorColor
                                          })
                }
            } else {
                clearTransportInfo()
            }
        } else {
            clearTransportInfo()
        }

        console.log("Scanned have changed", scannedQrs.length, transportId)
    }

    function clearTransportInfo() {
        transportName = ""
        dataManager.minimumDateModel.plotId = -1
        bagsMatch = undefined
    }

    // Used by QRScanner page. Not used here, however. It's only used on
    // Loading and Transport page
    property bool shouldPause: false

    title: Strings.sellAtTheLocalMarket

    proceedButtonEnabled: hasQrs && (transportId !== -1)
                          && (bagsMatch.hasConflict === false)

    Component.onCompleted: refreshData()

    function refreshData() {
        dataManager.minimumDateModel.plotId = -1
    }

    function proceed() {
        pageManager.enter(Enums.Page.SupplyChainSummary, {
                              "supplyChainPage": this,
                              "summary": summary(),
                              "proceedButtonText": Strings.proceed
                          });
    }

    function summary() {
        let bagCount = dataManager.actionController.bagCountInTransport(transportId)
        let allBagsMatch = (bagsMatch.fullMatch === true)

        var summary = [
                    Utility.createSummaryItem(
                        Strings.transportId,
                        transportName,
                        "", "",
                        GStyle.delegateHighlightColor4,
                        GStyle.fontHighlightColor4,
                        GStyle.fontHighlightColor4,
                        Enums.DelegateType.Standard,
                        false),
                    Utility.createSummaryItem(
                        "",
                        [
                            [
                                Strings.numberOfBagsSold,
                                Strings.numberOfBagsLeft
                            ],
                            [
                                bagsMatch.bagsFromReception.length,
                                bagsMatch.countBagsLeftOnTruck()
                            ],
                            [
                                ""
                            ],
                            [
                                Enums.Page.InvalidPage
                            ],
                            [
                                ""
                            ]
                        ],
                        "", "",
                        GStyle.delegateHighlightColor4,
                        GStyle.fontHighlightColor4,
                        GStyle.textPrimaryColor,
                        Enums.DelegateType.ColumnStack,
                        true),
                    Utility.createSummaryItem(
                        Strings.plateNumber,
                        dataManager.actionController.plateNumberInTransport(
                            transportId)
                        ),
                    Utility.createSummaryItem(
                        Strings.receptionDateCharcoal,
                        unloadingDateHeader.selectedDate.toLocaleDateString(
                            Qt.locale(), Strings.dateFormat)),
                    Utility.createSummaryItem(Strings.gpsCoordinates,
                                              gpsSource.coordinateString)
                ]
        return summary
    }

    function addAction() {
        let success = dataManager.actionController.registerLocalMarketReception(
                (gpsSource.coordinate? gpsSource.coordinate
                                     : QtPositioning.coordinate()),
                new Date,
                unloadingDateHeader.selectedDate,
                userManager.userData.code,
                transportId,
                scannedQrs
                )

        pageManager.backTo(Enums.Page.MainMenu)
    }

    CharcoalHeaders.CharcoalButtonHeader {
        id: scanAllBagsHeader

        Layout.fillWidth: true
        forceBoldValue: true
        valueFontSize: s(GStyle.titlePixelSize)

        text: Strings.scanBagsYouWantToSell
        extraText: hasQrs? Strings.greenBagCount.arg(scannedQrs.length) : ""
        iconVisible: hasQrs
        icon: bagsMatch !== undefined && bagsMatch.hasConflict?
                  GStyle.iconNoUrl : GStyle.checkGreenUrl

        onClicked: pageManager.enter(
                       Enums.Page.QRScanner,
                       {
                           "title": top.title,
                           "infoText": Strings.scanBagsInfoText,
                           "backToPage": Enums.Page.SupplyChainSellAtTheLocalMarket,
                           "infoImages": [ GStyle.bagsReceptionUrl ],
                           "scannedQrs": scannedQrs
                       })
    }

    CharcoalHeaders.CharcoalInputDateHeader {
        id: unloadingDateHeader
        Layout.fillWidth: true
        headerText: Strings.receptionDateCharcoal
        helpButtonVisible: true
        helpText: Strings.receptionUnloadingDateHelp
        minimumDate: dataManager.minimumDateModel.date
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
        inputText: (gpsSource.validCoordinate ? Helper.formatCoordinate(gpsSource.coordinateString) : gpsSource.errorMessage())
        iconSource: (gpsSource.validCoordinate ? GStyle.gpsOkImgUrl : GStyle.gpsFailedImgUrl)

        onClicked: gpsSource.update()
    }
}
