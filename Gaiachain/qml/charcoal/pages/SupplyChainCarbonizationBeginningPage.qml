import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../../common" as Common
import "../../headers" as Headers
import "../headers" as CharcoalHeaders
import "../items" as CharcoalItems
import "../../pages" as Pages

Pages.SupplyChainPageBase {
    id: top

    property var ovenDimensions: []
    property string volume: value

    onOvenDimensionsChanged: {
        console.log("Oven dimensions length:", ovenDimensions.length)

        volume = ""

        if (ovenDimensions.length === 3) {
            volume = dataManager.actionController.ovenVolume(
                        parseFloat(ovenDimensions[0]),
                        parseFloat(ovenDimensions[1]),
                        parseFloat(ovenDimensions[2]))
        } else if (ovenDimensions.length === 4) {
            volume = dataManager.actionController.ovenVolume(
                        parseFloat(ovenDimensions[0]),
                        parseFloat(ovenDimensions[1]),
                        parseFloat(ovenDimensions[2]),
                        parseFloat(ovenDimensions[3]))
        }
    }

    title: Strings.carbonizationBeginning

    // 1 - Traditional oven
    // 2 - Metallic oven
    // These ids come from Web :shrug:
    proceedButtonEnabled: (ovenIdHeader.inputText.length > 0
                           && plotIdComboBox.currentText.length > 0
                           && (ovenTypeComboBox.ovenType === "2"
                               || (ovenTypeComboBox.ovenType === "1"
                                   && ovenDimensions.length === 4)))

    Component.onCompleted: refreshData()

    function refreshData() {
        dataManager.unusedPlotIdsForCarbonizationModel.refresh()
        dataManager.ovenTypesModel.refresh()
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
        let dims = ovenDimensions
        let dimensionsString = dims[0] + " x " + dims[1] + " x " + dims[2]
        if (dims.length === 4) {
            dimensionsString += " x " + dims[3]
        }
        dimensionsString += "m"

        var summary = [
                    Utility.createSummaryItem(
                        Strings.ovenId,
                        [
                            [ ovenIdHeader.inputText ],
                            [ ovenTypeComboBox.currentText
                             + " - "
                             + volume + " m³"
                             //+ dimensionsString
                            ]
                        ],
                        "", "",
                        GStyle.delegateHighlightColor3,
                        GStyle.fontHighlightColor3,
                        GStyle.fontHighlightColor3,
                        Enums.DelegateType.Column,
                        false),
                    Utility.createSummaryItem(
                        "",
                        [
                            ovenDimensionsHeader.titles,
                            dims
                        ],
                        "", "",
                        GStyle.delegateHighlightColor3,
                        GStyle.fontHighlightColor3,
                        GStyle.fontHighlightColor3,
                        Enums.DelegateType.Row,
                        true),
                    Utility.createSummaryItem(
                        Strings.harvestId,
                        dataManager.actionController.generateHarvestId(
                            plotIdComboBox.currentText,
                            carbonizerIdInputHeader.inputText
                            ),
                        "", "",
                        GStyle.delegateHighlightColor2,
                        GStyle.fontHighlightColor2,
                        GStyle.fontHighlightColor2,
                        Enums.DelegateType.Standard,
                        true),
                    Utility.createSummaryItem(
                        Strings.plotId,
                        plotIdComboBox.currentText,
                        "", "",
                        GStyle.delegateHighlightColor,
                        GStyle.fontHighlightColor,
                        GStyle.fontHighlightColor,
                        Enums.DelegateType.Standard,
                        true),
                    Utility.createSummaryItem(Strings.carbonizerId,
                                              carbonizerIdInputHeader.inputText),
                    Utility.createSummaryItem(
                        Strings.beginningDate,
                        beginningDateHeader.selectedDate.toLocaleDateString(
                            Qt.locale(), Strings.dateFormat)),
                    Utility.createSummaryItem(Strings.ovenType,
                                              ovenTypeComboBox.currentText),
                    Utility.createSummaryItem(Strings.gpsCoordinates,
                                              gpsSource.coordinateString)
                ]
        return summary
    }

    function addAction() {
        dataManager.actionController.registerCarbonizationBeginning(
                    (gpsSource.coordinate? gpsSource.coordinate
                                         : QtPositioning.coordinate()),
                    new Date,
                    beginningDateHeader.selectedDate,
                    carbonizerIdInputHeader.inputText,
                    plotIdComboBox.currentText,
                    plotIdComboBox.currentId,
                    ovenIdHeader.inputText,
                    ovenTypeComboBox.ovenIdNumber,
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

        model: dataManager.unusedPlotIdsForCarbonizationModel

        onCurrentTextChanged: {
            ovenIdHeader.inputText = dataManager.actionController.nextOvenNumber(currentId)
            dataManager.minimumDateModel.plotId = currentId
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

        onCurrentTextChanged: {
            if (ovenType === "2") {
                ovenDimensions = dataManager.actionController.defaultOvenDimensions(
                            ovenIdNumber)
            } else {
                ovenDimensions = []
            }
        }
    }

    CharcoalHeaders.CharcoalButtonHeader {
        property var titles: ovenTypeComboBox.isTraditional?
            [ Strings.length, Strings.width, Strings.heightA, Strings.heightB ]
            : [ Strings.height, Strings.length, Strings.width ]

        id: ovenDimensionsHeader

        Layout.fillWidth: true

        headerText: Strings.ovenDimensions
        helpButtonVisible: true
        helpText: Strings.carbonizationBeginningOvenDimensionsHelp
        enabled: ovenTypeComboBox.isTraditional

        text: Strings.set + (volume.length === 0? "" : (" " + volume + "m³"))
        extraText: ""
        iconVisible: false

        onClicked: pageManager.enter(
                       Enums.Page.SetOvenMeasurements,
                       {
                           "backToPage": Enums.Page.SupplyChainCarbonizationBeginning,
                           "ovenDimensions": top.ovenDimensions
                       })
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
        inputText: (gpsSource.validCoordinate ? Helper.formatCoordinate(gpsSource.coordinateString) : gpsSource.errorMessage())
        iconSource: (gpsSource.validCoordinate ? GStyle.gpsOkImgUrl : GStyle.gpsFailedImgUrl)

        onClicked: gpsSource.update()
    }
}
