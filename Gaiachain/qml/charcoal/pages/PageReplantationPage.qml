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
import "../../popups" as Popups

Pages.SupplyChainPageBase {
    id: top

    title: Strings.replantation

    proceedButtonEnabled: (plotIdComboBox.currentText.length > 0
                           && numberOfTreesHeader.inputText.length > 0
                           && treeSpeciesComboBox.currentText.length > 0)

    Component.onCompleted: refreshData()

    function refreshData() {
        dataManager.unusedPlotIdsForReplantationModel.refresh()
        dataManager.treeSpeciesModel.refresh()
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
                    Utility.createSummaryItem(Strings.plotId, plotIdComboBox.currentText,
                                      "", "",
                                      GStyle.delegateHighlightColor,
                                      GStyle.fontHighlightColor,
                                      "",
                                      Enums.DelegateType.Standard),
                    Utility.createSummaryItem(Strings.numberOfTreesPlanted,
                                      numberOfTreesHeader.inputText),
                    Utility.createSummaryItem(Strings.treeSpecies,
                                      treeSpeciesComboBox.currentText),
                    Utility.createSummaryItem(Strings.userId,
                                      userIdInputHeader.inputText),
                    Utility.createSummaryItem(Strings.beginningDate,
                                      beginningDateHeader.selectedDate.toLocaleDateString(
                                          Qt.locale(), Strings.dateFormat)),
                    Utility.createSummaryItem(Strings.endingDate,
                                      endingDateHeader.selectedDate.toLocaleDateString(
                                          Qt.locale(), Strings.dateFormat)),
                    Utility.createSummaryItem(Strings.gpsCoordinates,
                                      gpsSource.coordinate.toString())
                ]

        return summary
    }

    function addAction() {
        dataManager.actionController.registerReplantation(
                    (gpsSource.coordinate? gpsSource.coordinate
                                         : QtPositioning.coordinate()),
                    new Date,
                    userManager.userData.code,
                    plotIdComboBox.currentText,
                    numberOfTreesHeader.inputText,
                    treeSpeciesComboBox.currentText,
                    beginningDateHeader.selectedDate,
                    endingDateHeader.selectedDate
                    )

        pageManager.enter(Enums.Page.MainMenu)
    }

    CharcoalHeaders.CharcoalComboBoxHeader {
        id: plotIdComboBox
        Layout.fillWidth: true
        headerText: Strings.plotId
        helpButtonVisible: true
        helpText: Strings.replantationPlotIdHelp
        popupTitle: Strings.selectPlotId

        checkIcon: GStyle.checkBlueUrl
        delegateTextColor: GStyle.fontHighlightColor

        model: dataManager.unusedPlotIdsForReplantationModel
    }

    Headers.InputHeader {
        id: numberOfTreesHeader
        Layout.fillWidth: true
        headerText: Strings.numberOfTreesPlanted
        helpButtonVisible: true
        helpText: Strings.replantationNumberOfTreesHelp
        validator: IntValidator {
            bottom: 0
            top: 32767
        }
    }

    CharcoalHeaders.CharcoalComboBoxHeader {
        id: treeSpeciesComboBox
        Layout.fillWidth: true
        headerText: Strings.treeSpecies
        helpButtonVisible: true
        helpText: Strings.replantationTreeSpeciesHelp
        popupTitle: Strings.selectTreeSpecies

        model: dataManager.treeSpeciesModel
    }

    CharcoalHeaders.UserInfoHeader {
        id: userIdInputHeader
        Layout.fillWidth: true
        headerText: Strings.userId
        helpText: Strings.replantationUserIdHelp
    }

    CharcoalHeaders.CharcoalInputDateHeader {
        id: beginningDateHeader
        Layout.fillWidth: true
        headerText: Strings.beginningDate
        helpButtonVisible: true
        helpText: Strings.replantationBeginningDateHelp
    }

    CharcoalHeaders.CharcoalInputDateHeader {
        id: endingDateHeader
        Layout.fillWidth: true
        headerText: Strings.endingDate
        helpButtonVisible: true
        helpText: Strings.replantationEndingDateHelp
        minimumDate: beginningDateHeader.selectedDate
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
        helpText: Strings.replantationGpsHelp

        Layout.fillWidth: true

        headerText: Strings.gpsCoordinates
        inputText: (gpsSource.validCoordinate ? Helper.formatCoordinate(gpsSource.coordinate.toString()) : gpsSource.errorMessage())
        iconSource: (gpsSource.validCoordinate ? GStyle.gpsOkImgUrl : GStyle.gpsFailedImgUrl)

        onClicked: gpsSource.update()
    }
}
