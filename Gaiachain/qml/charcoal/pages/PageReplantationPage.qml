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
                    createSummaryItem(Strings.plotId, plotIdComboBox.currentText,
                                      "", "",
                                      Pages.SupplyChainPageBase.Standard,
                                      GStyle.delegateHighlightColor,
                                      GStyle.fontHighlightColor),
                    createSummaryItem(Strings.numberOfTreesPlanted,
                                      numberOfTreesHeader.inputText),
                    createSummaryItem(Strings.treeSpecies,
                                      treeSpeciesComboBox.currentText),
                    createSummaryItem(Strings.userId,
                                      userIdInputHeader.inputText),
                    createSummaryItem(Strings.beginningDate,
                                      beginningDateHeader.selectedDate.toLocaleDateString(
                                          Qt.locale(), Strings.dateFormat)),
                    createSummaryItem(Strings.endingDate,
                                      endingDateHeader.selectedDate.toLocaleDateString(
                                          Qt.locale(), Strings.dateFormat)),
                    createSummaryItem(Strings.gpsCoordinates,
                                      gpsSource.coordinate.toString())
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

    CharcoalHeaders.CharcoalComboBoxHeader {
        id: plotIdComboBox
        Layout.fillWidth: true
        headerText: Strings.plotId
        helpButtonVisible: true
        helpText: Strings.replantationPlotIdHelp

        checkIcon: GStyle.checkBlueUrl
        delegateTextColor: GStyle.fontHighlightColor

        model: [ "AM003PM/0595112/04-03-2020" ]
    }

    Headers.InputHeader {
        id: numberOfTreesHeader
        Layout.fillWidth: true
        headerText: Strings.numberOfTreesPlanted
        helpButtonVisible: true
        helpText: Strings.replantationNumberOfTreesHelp
        validator: IntValidator {
            bottom: 0
        }
    }

    CharcoalHeaders.CharcoalComboBoxHeader {
        id: treeSpeciesComboBox
        Layout.fillWidth: true
        headerText: Strings.treeSpecies
        helpButtonVisible: true
        helpText: Strings.replantationTreeSpeciesHelp

        model: [ "Cassia siamea", "Technona grandis" ]
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
