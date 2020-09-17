import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../../common" as Common
import "../../headers" as Headers
import "../headers" as CharcoalHeaders
import "../../pages" as Pages

Pages.SupplyChainPageBase {
    id: top

    title: Strings.loggingEnding

    proceedButtonEnabled: (plotIdComboBox.currentText.length > 0
                           && numberOfTreesHeader.counterValue.length > 0)

    Component.onCompleted: refreshData()

    function refreshData() {
        dataManager.unusedPlotIdsForLoggingEndingModel.refresh()
        dataManager.minimumDateModel.plotId = -1
    }

    function proceed() {
        pageManager.enter(Enums.Page.SupplyChainSummary, {
                              "supplyChainPage": this,
                              "summary": summary(),
                              "proceedButtonText": Strings.updatePlotId
                          });
    }

    function summary() {
        var summary = [
                    Utility.createSummaryItem(
                        Strings.plotId, plotIdComboBox.currentText,
                        "", "",
                        GStyle.delegateHighlightColor,
                        GStyle.fontHighlightColor,
                        GStyle.fontHighlightColor,
                        Enums.DelegateType.Standard,
                        true),
                    Utility.createSummaryItem(Strings.malebiRepsId,
                                              repsIdInputHeader.inputText),
                    Utility.createSummaryItem(
                        Strings.endingDate,
                        endingDateHeader.selectedDate.toLocaleDateString(
                            Qt.locale(), Strings.dateFormat)),
                    Utility.createSummaryItem(Strings.numberOfTrees,
                                              numberOfTreesHeader.counterValue),
                    Utility.createSummaryItem(Strings.gpsCoordinates,
                                              gpsSource.coordinateString)
                ]

        return summary
    }

    function addAction() {
        dataManager.actionController.registerLoggingEnding(
                    (gpsSource.coordinate? gpsSource.coordinate
                                         : QtPositioning.coordinate()),
                    new Date,
                    endingDateHeader.selectedDate,
                    repsIdInputHeader.inputText,
                    plotIdComboBox.currentId,
                    numberOfTreesHeader.counterValue)

        pageManager.enter(Enums.Page.MainMenu)
    }

    CharcoalHeaders.CharcoalComboBoxHeader {
        id: plotIdComboBox
        Layout.fillWidth: true
        headerText: Strings.plotId
        helpButtonVisible: true
        helpText: Strings.loggingEndingPlotIdHelp
        popupTitle: Strings.selectPlotId

        checkIcon: GStyle.checkBlueUrl
        delegateTextColor: GStyle.fontHighlightColor

        model: dataManager.unusedPlotIdsForLoggingEndingModel

        onCurrentTextChanged: dataManager.minimumDateModel.plotId = currentId
    }

    CharcoalHeaders.UserInfoHeader {
        id: repsIdInputHeader
        Layout.fillWidth: true
        helpText: Strings.loggingEndingRepsIdHelp
    }

    CharcoalHeaders.CharcoalInputDateHeader {
        id: endingDateHeader
        Layout.fillWidth: true
        headerText: Strings.endingDate
        helpButtonVisible: true
        helpText: Strings.loggingEndingEndingDateHelp
        minimumDate: dataManager.minimumDateModel.date
    }

    CharcoalHeaders.TreeCountHeader {
        id: numberOfTreesHeader
        Layout.fillWidth: true

        plusHelpText: Strings.loggingEndingNumberOfTreesHelp
        counterHelpText: Strings.loggingEndingNumberOfTreesHelp

        plusHeaderText: Strings.numberOfTreesCut
        plusButtonText: Strings.tapToAddATreeBeingCut

        counterHeaderText: Strings.totalCountOfTreesCut
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
        helpText: Strings.loggingEndingGpsHelp

        Layout.fillWidth: true

        headerText: Strings.gpsCoordinates
        inputText: (gpsSource.validCoordinate ? Helper.formatCoordinate(gpsSource.coordinateString) : gpsSource.errorMessage())
        iconSource: (gpsSource.validCoordinate ? GStyle.gpsOkImgUrl : GStyle.gpsFailedImgUrl)

        onWholeInputClicked: gpsSource.update()
    }
}
