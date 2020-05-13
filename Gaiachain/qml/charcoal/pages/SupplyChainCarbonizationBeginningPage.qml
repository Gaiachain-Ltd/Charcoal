import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.types 1.0

import "../../common" as Common
import "../../headers" as Headers
import "../headers" as CharcoalHeaders
import "../../pages" as Pages

Pages.SupplyChainPageBase {
    id: top

    title: Strings.carbonizationBeginning

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
                    createSummaryItem(Strings.harvestId,
                                      (plotIdComboBox.currentText
                                       + "/" + carbonizerIdInputHeader.inputText),
                                      "", "",
                                      GStyle.delegateHighlightColor2,
                                      GStyle.fontHighlightColor2),
                    createSummaryItem(Strings.plotId, plotIdComboBox.currentText,
                                      "", "",
                                      GStyle.delegateHighlightColor,
                                      GStyle.fontHighlightColor),
                    createSummaryItem(Strings.ovenId, ovenIdHeader.inputText,
                                      "", "",
                                      GStyle.delegateHighlightColor3,
                                      GStyle.fontHighlightColor3),
                    createSummaryItem(Strings.carbonizerId,
                                      carbonizerIdInputHeader.inputText),
                    createSummaryItem(Strings.beginningDate,
                                      beginningDateHeader.selectedDate.toLocaleDateString(
                                          Qt.locale(), Strings.dateFormat)),
                    createSummaryItem(Strings.ovenType, ovenTypeComboBox.currentText),
                    createSummaryItem(Strings.ovenDimensions,
                                      {
                                         "a": ovenDimensionsHeader.heightText,
                                          "b": ovenDimensionsHeader.lengthText,
                                          "c": ovenDimensionsHeader.widthText
                                      }
                                      ),
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

    Headers.ComboBoxHeader {
        id: plotIdComboBox
        Layout.fillWidth: true
        headerText: Strings.plotId
        helpButtonVisible: true
        helpText: Strings.carbonizationBeginningPlotIdHelp
    }

    Headers.InputHeader {
        id: ovenIdHeader
        Layout.fillWidth: true
        headerText: Strings.ovenId
        helpButtonVisible: true
        helpText: Strings.carbonizationBeginningOvenIdHelp
    }

    CharcoalHeaders.UserInfoHeader {
        id: carbonizerIdInputHeader
        Layout.fillWidth: true
        headerText: Strings.carbonizerId
        helpText: Strings.carbonizationBeginningCarbonizerIdHelp
    }

    Headers.InputDateHeader {
        id: beginningDateHeader
        Layout.fillWidth: true
        headerText: Strings.beginningDate
        helpButtonVisible: true
        helpText: Strings.carbonizationBeginningBeginningDateHelp
    }

    Headers.ComboBoxHeader {
        id: ovenTypeComboBox
        Layout.fillWidth: true
        headerText: Strings.ovenType
        helpButtonVisible: true
        helpText: Strings.carbonizationBeginningOvenTypeHelp
    }

    Headers.DimensionsHeader {
        id: ovenDimensionsHeader
        Layout.fillWidth: true
        headerText: Strings.ovenDimensions
        helpButtonVisible: true
        helpText: Strings.carbonizationBeginningOvenDimensionsHelp
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
        helpText: Strings.carbonizationBeginningGpsHelp

        Layout.fillWidth: true

        headerText: Strings.gpsCoordinates
        inputText: (gpsSource.validCoordinate ? Helper.formatCoordinate(gpsSource.coordinate.toString()) : gpsSource.errorMessage())
        iconSource: (gpsSource.validCoordinate ? GStyle.gpsOkImgUrl : GStyle.gpsFailedImgUrl)

        onClicked: gpsSource.update()
    }
}
