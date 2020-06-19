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
import "../items" as CharcoalItems
import "../../pages" as Pages

Pages.SupplyChainPageBase {
    id: top

    title: Strings.carbonizationBeginning

    proceedButtonEnabled: (plotIdComboBox.currentText.length > 0
                           && (ovenTypeComboBox.ovenName === "metallic"
                           || (ovenTypeComboBox.ovenName === "traditional"
                               && ovenDimensionsHeader.isEmpty === false)))

    Component.onCompleted: refreshData()

    function refreshData() {
        dataManager.unusedPlotIdsModel.refresh()
        dataManager.ovenTypesModel.refresh()
        dataManager.minimumDateModel.plotId = ""
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
                                      dataManager.actionController.generateHarvestId(
                                          plotIdComboBox.currentText,
                                          carbonizerIdInputHeader.inputText
                                          ),
                                      "", "",
                                      Pages.SupplyChainPageBase.Standard,
                                      GStyle.delegateHighlightColor2,
                                      GStyle.fontHighlightColor2),
                    createSummaryItem(Strings.plotId, plotIdComboBox.currentText,
                                      "", "",
                                      Pages.SupplyChainPageBase.Standard,
                                      GStyle.delegateHighlightColor,
                                      GStyle.fontHighlightColor),
                    createSummaryItem(Strings.ovenId, ovenIdHeader.inputText,
                                      "", "",
                                      Pages.SupplyChainPageBase.Standard,
                                      GStyle.delegateHighlightColor3,
                                      GStyle.fontHighlightColor3),
                    createSummaryItem(Strings.carbonizerId,
                                      carbonizerIdInputHeader.inputText),
                    createSummaryItem(Strings.beginningDate,
                                      beginningDateHeader.selectedDate.toLocaleDateString(
                                          Qt.locale(), Strings.dateFormat)),
                    createSummaryItem(Strings.ovenType, ovenTypeComboBox.currentText),
                    createSummaryItem(Strings.ovenDimensions,
                                      [ovenDimensionsHeader.titles,
                                       ovenTypeComboBox.ovenName === "metallic"?
                                           dataManager.actionController.defaultOvenDimensions(
                                               ovenTypeComboBox.ovenName)
                                         : ovenDimensionsHeader.values],
                                      "", "",
                                      Pages.SupplyChainPageBase.Row),
                    createSummaryItem(Strings.gpsCoordinates, gpsSource.coordinate.toString())
                ]
        return summary
    }

    function addAction() {
        dataManager.actionController.registerCarbonizationBeginning(
                    (gpsSource.coordinate? gpsSource.coordinate
                                         : QtPositioning.coordinate()),
                    beginningDateHeader.selectedDate,
                    carbonizerIdInputHeader.inputText,
                    plotIdComboBox.currentText,
                    ovenIdHeader.inputText,
                    ovenTypeComboBox.ovenName,
                    ovenDimensionsHeader.values)

        pageManager.enter(Enums.Page.MainMenu)
    }

    CharcoalHeaders.CharcoalComboBoxHeader {
        id: plotIdComboBox
        Layout.fillWidth: true
        headerText: Strings.plotId
        helpButtonVisible: true
        helpText: Strings.carbonizationBeginningPlotIdHelp
        popupTitle: Strings.selectPlotId

        checkIcon: GStyle.checkBlueUrl
        delegateTextColor: GStyle.fontHighlightColor

        model: dataManager.unusedPlotIdsModel

        onCurrentTextChanged: {
            ovenIdHeader.inputText = dataManager.actionController.nextOvenNumber(currentText)
            dataManager.minimumDateModel.plotId = currentText
        }
    }

    Headers.InputHeader {
        id: ovenIdHeader
        Layout.fillWidth: true
        headerText: Strings.ovenId
        helpButtonVisible: true
        helpText: Strings.carbonizationBeginningOvenIdHelp
        readOnly: true
    }

    CharcoalHeaders.UserInfoHeader {
        id: carbonizerIdInputHeader
        Layout.fillWidth: true
        headerText: Strings.carbonizerId
        helpText: Strings.carbonizationBeginningCarbonizerIdHelp
    }

    CharcoalHeaders.CharcoalInputDateHeader {
        id: beginningDateHeader
        Layout.fillWidth: true
        headerText: Strings.beginningDate
        helpButtonVisible: true
        helpText: Strings.carbonizationBeginningBeginningDateHelp
        minimumDate: dataManager.minimumDateModel.date
    }

    CharcoalHeaders.OvenTypeComboBox {
        id: ovenTypeComboBox
        Layout.fillWidth: true
        headerText: Strings.ovenType
        helpButtonVisible: true
        helpText: Strings.carbonizationBeginningOvenTypeHelp
        popupTitle: Strings.selectOvenType

        model: dataManager.ovenTypesModel
    }

    Headers.RowHeader {
        property bool isEmpty: true

        onValueChanged: {
            let emptyCheck = false;
            for (let value of values) {
                if (value.length === 0) {
                    emptyCheck = true
                    break
                }
            }

            isEmpty = emptyCheck
        }

        id: ovenDimensionsHeader
        Layout.fillWidth: true
        headerText: Strings.ovenDimensions
        helpButtonVisible: true
        helpText: Strings.carbonizationBeginningOvenDimensionsHelp
        titles: [ Strings.height, Strings.length, Strings.width ]
        enabled: ovenTypeComboBox.isTraditional
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
