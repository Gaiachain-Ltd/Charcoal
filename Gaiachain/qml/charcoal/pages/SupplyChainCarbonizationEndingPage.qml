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

    title: Strings.carbonizationEnding

    proceedButtonEnabled: (harvestIdComboBox.currentText.length > 0
                           && ovenIdComboBox.currentText.length > 0)

    Component.onCompleted: refreshData()

    function refreshData() {
        dataManager.unusedHarvestIdsModel.refresh()
        dataManager.ovensModel.plotId = ""
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
                    dataManager.createSummaryItem(Strings.harvestId,
                                      harvestIdComboBox.currentText,
                                      "", "",
                                      GStyle.delegateHighlightColor2,
                                      GStyle.fontHighlightColor2,
                                      "",
                                      Enums.DelegateType.Standard),
                    dataManager.createSummaryItem(Strings.plotId,
                                      dataManager.actionController.getPlotId(harvestIdComboBox.currentText),
                                      "", "",
                                      GStyle.delegateHighlightColor,
                                      GStyle.fontHighlightColor,
                                      "",
                                      Enums.DelegateType.Standard),
                    dataManager.createSummaryItem(Strings.ovenId,
                                      [
                                          ovenIdComboBox.letters,
                                          ovenIdComboBox.descriptions
                                      ],
                                      "", "",
                                      GStyle.delegateHighlightColor3,
                                      GStyle.fontHighlightColor3,
                                      "",
                                      Enums.DelegateType.Column),
                    dataManager.createSummaryItem(Strings.carbonizerId,
                                      carbonizerIdInputHeader.inputText),
                    dataManager.createSummaryItem(Strings.carbonizationEndingDate,
                                      endingDateHeader.selectedDate.toLocaleDateString(
                                          Qt.locale(), Strings.dateFormat)),
                    dataManager.createSummaryItem(Strings.gpsCoordinates, gpsSource.coordinate.toString())
                ]
        return summary
    }

    function addAction() {
        dataManager.actionController.registerCarbonizationEnding(
                    (gpsSource.coordinate? gpsSource.coordinate
                                         : QtPositioning.coordinate()),
                    new Date,
                    endingDateHeader.selectedDate,
                    carbonizerIdInputHeader.inputText,
                    harvestIdComboBox.currentText,
                    dataManager.actionController.getPlotId(harvestIdComboBox.currentText),
                    ovenIdComboBox.ids
                    )

        pageManager.enter(Enums.Page.MainMenu)
    }

    CharcoalHeaders.CharcoalComboBoxHeader {
        id: harvestIdComboBox
        Layout.fillWidth: true
        headerText: Strings.harvestId
        helpButtonVisible: true
        helpText: Strings.carbonizationEndingHarvestIdHelp
        popupTitle: Strings.selectHarvestId

        checkIcon: GStyle.checkGreenUrl
        delegateTextColor: GStyle.fontHighlightColor2

        model: dataManager.unusedHarvestIdsModel

        onCurrentTextChanged: {
            // Refresh available ovens
            dataManager.ovensModel.plotId = dataManager.actionController.getPlotId(currentText)
            dataManager.minimumDateModel.plotId = currentText
        }
    }

    CharcoalHeaders.OvenSelectionComboBox {
        id: ovenIdComboBox
        Layout.fillWidth: true
        headerText: Strings.ovenId
        helpButtonVisible: true
        helpText: Strings.carbonizationEndingOvenIdHelp
        popupTitle: Strings.selectOvenId

        checkIcon: GStyle.checkOrangeUrl
        delegateTextColor: GStyle.fontHighlightColor3

        multiSelect: true

        model: dataManager.ovensModel
    }

    CharcoalHeaders.UserInfoHeader {
        id: carbonizerIdInputHeader
        Layout.fillWidth: true
        headerText: Strings.carbonizerId
        helpText: Strings.carbonizationEndingCarbonizerIdHelp
    }

    CharcoalHeaders.CharcoalInputDateHeader {
        id: endingDateHeader
        Layout.fillWidth: true
        headerText: Strings.carbonizationEndingDate
        helpButtonVisible: true
        helpText: Strings.carbonizationEndingEndingDateHelp
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
        helpText: Strings.carbonizationEndingGpsHelp

        Layout.fillWidth: true

        headerText: Strings.gpsCoordinates
        inputText: (gpsSource.validCoordinate ? Helper.formatCoordinate(gpsSource.coordinate.toString()) : gpsSource.errorMessage())
        iconSource: (gpsSource.validCoordinate ? GStyle.gpsOkImgUrl : GStyle.gpsFailedImgUrl)

        onClicked: gpsSource.update()
    }
}
