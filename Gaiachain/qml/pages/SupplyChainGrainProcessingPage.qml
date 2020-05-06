import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.types 1.0

import "../items" as Items
import "../headers" as Headers
import "../components" as Components
import "../pages" as Pages

Pages.SupplyChainPage {
    id: top

    title: Strings.supplyChainMenuActionGrainProcessing

    validPageData: harvestIdComboBox.currentText !== Strings.empty

    Component.onCompleted: refreshData()

    function refreshData() {
        dataManager.fetchLastActionPackageEvents(Enums.SupplyChainAction.Harvest);
        lastActionHarvestModel.resetFilter()
    }

    function proceed() {
        pageManager.enter(Enums.Page.SupplyChainSummary, { "supplyChainPage": this, "summary": summary() })
    }

    function summary() {
        return [
            createSummaryItem(Strings.gpsCoordinates, gpsCoordinates, GStyle.gpsImgUrl),
            createSummaryItem(Strings.harvestId, harvestIdComboBox.currentText),
            createSummaryItem(Strings.breakingDate, breakingDateInputDate.selectedDate.toLocaleDateString(Qt.locale(), Strings.dateFormat)),
            createSummaryItem(Strings.endFermentationDate, endFermentationDateInputDate.selectedDate.toLocaleDateString(Qt.locale(), Strings.dateFormat)),
            createSummaryItem(Strings.estimatedBeansVolume + " (" + Strings.notRequired + ")", Strings.kg.arg(Helper.minusIfNotDefined(estimatedBeansVolumeInputHeader.inputText)))
        ]
    }

    function addAction() {
        showOverlay()

        var harvestId = harvestIdComboBox.currentText
        var breakingDate = breakingDateInputDate.selectedDate
        var endFermentationDate = endFermentationDateInputDate.selectedDate
        var estimatedBeansVolume = estimatedBeansVolumeInputHeader.inputText

        var properties = {
            [PackageDataProperties.BreakingDate]: breakingDate,
            [PackageDataProperties.FermentationEndDate]: endFermentationDate
        }
        if (estimatedBeansVolume) {
            properties[PackageDataProperties.EstimatedBeansVolume] = estimatedBeansVolume
        }

        top.packageId = harvestId
        dataManager.addAction(harvestId,
                              Enums.SupplyChainAction.GrainProcessing,
                              coordinate(),
                              new Date,
                              properties)
    }

    Headers.ComboBoxHeader {
        id: harvestIdComboBox

        Layout.fillWidth: true

        headerText: Strings.harvestId

        displayRole: "packageId"
        model: lastActionHarvestModel
    }

    Headers.InputDateHeader {
        id: breakingDateInputDate

        Layout.fillWidth: true

        headerText: Strings.breakingDate
    }

    Headers.InputDateHeader {
        id: endFermentationDateInputDate

        Layout.fillWidth: true

        headerText: Strings.endFermentationDate
    }

    Headers.InputHeader {
        id: estimatedBeansVolumeInputHeader

        Layout.fillWidth: true

        validator: IntValidator {}
        additionalInputMethodHints: Qt.ImhDigitsOnly

        optional: true
        headerText: Strings.estimatedBeansVolume + " (" + Strings.notRequired + ")"
        headerTextColor: GStyle.notRequiredTextInputColor

        iconSource: GStyle.rightArrowImgUrl

        placeholderText: Strings.typeHere + "..."
    }
}
