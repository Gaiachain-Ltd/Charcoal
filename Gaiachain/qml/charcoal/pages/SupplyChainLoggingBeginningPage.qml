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

    title: Strings.loggingBeginning

    proceedButtonEnabled: (parcelComboBox.currentText.length > 0
                           && villageComboBox.currentText.length > 0
                           && treeSpeciesComboBox.currentText.length > 0)

    Component.onCompleted: refreshData()

    function refreshData() {
        dataManager.parcelsModel.refresh()
        dataManager.villagesModel.refresh()
        dataManager.treeSpeciesModel.refresh()
    }

    function proceed() {
        pageManager.enter(Enums.Page.SupplyChainSummary, {
                              "supplyChainPage": this,
                              "summary": summary(),
                              "proceedButtonText": Strings.createPlotId
                          });
    }

    function summary() {
        var summary = [
                    Utility.createSummaryItem(Strings.plotId,
                                      dataManager.actionController.generatePlotId(
                                          repsIdInputHeader.inputText,
                                          parcelComboBox.currentText,
                                          beginningDateHeader.selectedDate
                                          ),
                                      "", "",
                                      GStyle.delegateHighlightColor,
                                      GStyle.fontHighlightColor,
                                      "",
                                      Enums.DelegateType.Standard),
                    Utility.createSummaryItem(Strings.parcel, parcelComboBox.currentText),
                    Utility.createSummaryItem(Strings.malebiRepsId, repsIdInputHeader.inputText),
                    Utility.createSummaryItem(Strings.village, villageComboBox.currentText),
                    Utility.createSummaryItem(Strings.treeSpecies, treeSpeciesComboBox.currentText),
                    Utility.createSummaryItem(Strings.beginningDate,
                                      beginningDateHeader.selectedDate.toLocaleDateString(
                                          Qt.locale(), Strings.dateFormat)),
                    Utility.createSummaryItem(Strings.gpsCoordinates, gpsSource.coordinate.toString())
                ]

        return summary
    }

    function addAction() {
        dataManager.actionController.registerLoggingBeginning(
                    (gpsSource.coordinate? gpsSource.coordinate
                                         : QtPositioning.coordinate()),
                    new Date,
                    beginningDateHeader.selectedDate,
                    repsIdInputHeader.inputText,
                    parcelComboBox.currentText,
                    villageComboBox.currentText,
                    treeSpeciesComboBox.currentText)

        pageManager.enter(Enums.Page.MainMenu)
    }

    CharcoalHeaders.CharcoalComboBoxHeader {
        id: parcelComboBox
        Layout.fillWidth: true
        headerText: Strings.parcel
        helpButtonVisible: true
        helpText: Strings.loggingBeginningParcelHelp
        popupTitle: Strings.selectParcel

        model: dataManager.parcelsModel
    }

    CharcoalHeaders.UserInfoHeader {
        id: repsIdInputHeader
        Layout.fillWidth: true
        helpText: Strings.loggingBeginningRepsIdHelp
    }

    CharcoalHeaders.CharcoalComboBoxHeader {
        id: villageComboBox
        Layout.fillWidth: true
        headerText: Strings.village
        helpButtonVisible: true
        helpText: Strings.loggingBeginningVillageHelp
        popupTitle: Strings.selectVillage

        model: dataManager.villagesModel
    }

    CharcoalHeaders.CharcoalComboBoxHeader {
        id: treeSpeciesComboBox
        Layout.fillWidth: true
        headerText: Strings.treeSpecies
        helpButtonVisible: true
        helpText: Strings.loggingBeginningTreeSpeciesHelp
        popupTitle: Strings.selectTreeSpecies

        model: dataManager.treeSpeciesModel
    }

    CharcoalHeaders.CharcoalInputDateHeader {
        id: beginningDateHeader
        Layout.fillWidth: true
        headerText: Strings.beginningDate
        helpButtonVisible: true
        helpText: Strings.loggingBeginningDateHelp
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
        helpText: Strings.loggingBeginningGpsHelp

        Layout.fillWidth: true

        headerText: Strings.gpsCoordinates
        inputText: (gpsSource.validCoordinate ? Helper.formatCoordinate(gpsSource.coordinate.toString()) : gpsSource.errorMessage())
        iconSource: (gpsSource.validCoordinate ? GStyle.gpsOkImgUrl : GStyle.gpsFailedImgUrl)

        onClicked: gpsSource.update()
    }
}
