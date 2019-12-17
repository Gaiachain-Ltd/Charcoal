import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.modelhelper 1.0
import com.gaiachain.packagedata 1.0

import "../items" as Items
import "../components" as Components
import "../pages" as Pages

Pages.SupplyChainPage {
    id: top

    title: Strings.supplyChainMenuActionHarvest

    validPageData: !(parcelCodesComboBox.currentText === Strings.empty ||
                     producerIdComboBox.currentText === Strings.empty ||
                     producerNameComboBox.currentText === Strings.empty ||
                     villageInputHeader.inputText === Strings.empty)

    Component.onCompleted: refreshData()
    Component.onDestruction: parcelsModel.producerId = 0

    function refreshData() {
        sessionManager.getProducers()
    }

    function proceed() {
        showOverlay()

        var parcelCode = parcelCodesComboBox.currentText
        var parcelId = ModelHelper.getData(parcelCode, "id", parcelCodesComboBox.model)
        var harvestDate = inputHarvestDate.selectedDate

        var harvestId = dataManager.generateHarvestId(harvestDate, parcelCode)

        var properties = {
            [PackageDataProperties.ParcelId]: parcelId,
            [PackageDataProperties.HarvestDate]: harvestDate
        }

        top.packageId = harvestId
        dataManager.addAction(harvestId,
                              Enums.SupplyChainAction.Harvest,
                              coordinate(),
                              new Date,
                              properties)
    }

    Items.ComboBoxHeader {
        id: producerNameComboBox

        Layout.fillWidth: true

        headerText: Strings.nameOfProducer

        onActivated: {
            producerIdComboBox.currentIndex = currentIndex

            var village = ModelHelper.getData(currentIndex, "village", model)
            villageInputHeader.inputText = village

            var producerId = ModelHelper.getData(currentIndex, "id", model)
            parcelsModel.producerId = producerId
            parcelCodesComboBox.updateCurrentIndex()
        }

        displayRole: "name"
        model: producersModel
    }

    Items.ComboBoxHeader {
        id: producerIdComboBox

        Layout.fillWidth: true

        headerText: Strings.producerIdNumber

        onActivated: {
            producerNameComboBox.currentIndex = currentIndex

            var village = ModelHelper.getData(currentIndex, "village", model)
            villageInputHeader.inputText = village

            var producerId = ModelHelper.getData(currentIndex, "id", model)
            parcelsModel.producerId = producerId
            parcelCodesComboBox.updateCurrentIndex()
        }

        displayRole: "code"
        model: producersModel
    }

    Items.InputHeader {
        id: villageInputHeader

        Layout.fillWidth: true

        readOnly: true
        headerText: Strings.village
        placeholderText: enabled ? Strings.empty : Strings.selectedAutomatically
        enabled: inputText !== Strings.empty
    }

    Items.ComboBoxHeader {
        id: parcelCodesComboBox

        Layout.fillWidth: true

        headerText: Strings.parcelCode
        placeholderText: enabled ? Strings.toSelect : Strings.selectProducer
        enabled: count !== 0

        displayRole: "code"
        model: parcelsModel

        function updateCurrentIndex() {
            currentIndex = (count === 1) ? 0 : -1
        }
    }

    Items.InputDateHeader {
        id: inputHarvestDate

        Layout.fillWidth: true

        headerText: Strings.harvestDate
    }
}
