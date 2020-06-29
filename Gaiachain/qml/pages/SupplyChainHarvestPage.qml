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

    title: Strings.supplyChainMenuActionHarvest

    validPageData: !(parcelCodesComboBox.currentText === Strings.empty ||
                     producerCodeComboBox.currentText === Strings.empty ||
                     producerNameComboBox.currentText === Strings.empty ||
                     villageInputHeader.inputText === Strings.empty)

    Component.onCompleted: refreshData()
    Component.onDestruction: parcelsModel.producerId = 0

    function refreshData() {
        sessionManager.getProducers()
    }

    function proceed() {
        pageManager.enter(Enums.Page.SupplyChainSummary, { "supplyChainPage": this, "summary": summary() })
    }

    function summary() {
        return [
            Utility.createSummaryItem(Strings.gpsCoordinates, gpsCoordinates, GStyle.gpsImgUrl),
            Utility.createSummaryItem(Strings.nameOfProducer, producerNameComboBox.currentText),
            Utility.createSummaryItem(Strings.producerIdNumber, producerCodeComboBox.currentText),
            Utility.createSummaryItem(Strings.village, villageInputHeader.inputText),
            Utility.createSummaryItem(Strings.parcelCode, parcelCodesComboBox.currentText),
            Utility.createSummaryItem(Strings.harvestDate, inputHarvestDate.selectedDate.toLocaleDateString(Qt.locale(), Strings.dateFormat))
        ]
    }

    function addAction() {
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

        console.log("ADDED", top.packageId, harvestId)
    }

    Headers.ComboBoxHeader {
        id: producerNameComboBox

        Layout.fillWidth: true

        headerText: Strings.nameOfProducer

        onActivated: {
            var producerCode = ModelHelper.getData(index, "code", filteredModel)
            producerCodeComboBox.currentIndex = ModelHelper.findRow("code", producerCode, producerCodeComboBox.model)

            var village = ModelHelper.getData(index, "village", filteredModel)
            villageInputHeader.inputText = village

            var producerId = ModelHelper.getData(index, "id", filteredModel)
            parcelsModel.producerId = producerId
            parcelCodesComboBox.updateCurrentIndex()
        }

        displayRole: "name"
        model: producersModel
    }

    Headers.ComboBoxHeader {
        id: producerCodeComboBox

        Layout.fillWidth: true

        headerText: Strings.producerIdNumber

        onActivated: {
            var producerName = ModelHelper.getData(index, "name", filteredModel)
            producerNameComboBox.currentIndex = ModelHelper.findRow("name", producerName, producerNameComboBox.model)

            var village = ModelHelper.getData(index, "village", filteredModel)
            villageInputHeader.inputText = village

            var producerId = ModelHelper.getData(index, "id", filteredModel)
            parcelsModel.producerId = producerId
            parcelCodesComboBox.updateCurrentIndex()
        }

        displayRole: "code"
        model: producersModel
    }

    Headers.InputHeader {
        id: villageInputHeader

        Layout.fillWidth: true

        readOnly: true
        headerText: Strings.village
        placeholderText: enabled ? Strings.empty : Strings.selectedAutomatically
        enabled: inputText !== Strings.empty
    }

    Headers.ComboBoxHeader {
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

    Headers.InputDateHeader {
        id: inputHarvestDate

        Layout.fillWidth: true

        headerText: Strings.harvestDate
    }
}
