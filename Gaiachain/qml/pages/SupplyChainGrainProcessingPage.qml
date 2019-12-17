import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.packagedata 1.0

import "../items" as Items
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

    Items.ComboBoxHeader {
        id: harvestIdComboBox

        Layout.fillWidth: true

        headerText: Strings.harvestId

        displayRole: "packageId"
        model: lastActionHarvestModel
    }

    Items.InputDateHeader {
        id: breakingDateInputDate

        Layout.fillWidth: true

        headerText: Strings.breakingDate
    }

    Items.InputDateHeader {
        id: endFermentationDateInputDate

        Layout.fillWidth: true

        headerText: Strings.endFermentationDate
    }

    Items.InputHeader {
        id: estimatedBeansVolumeInputHeader

        Layout.fillWidth: true

        validator: IntValidator {}
        inputMethodHints: Qt.ImhDigitsOnly

        optional: true
        headerText: Strings.estimatedBeansVolume + " (" + Strings.notRequired + ")"
        headerTextColor: Style.notRequiredTextInputColor

        iconSource: Style.rightArrowImgUrl

        placeholderText: Strings.typeHere + "..."
    }
}
