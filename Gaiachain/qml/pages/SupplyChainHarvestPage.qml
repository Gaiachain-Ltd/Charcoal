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

    proceedButtonEnabled: !(parcelCodesComboBox.currentText === Strings.empty ||
                            producerIdComboBox.currentText === Strings.empty ||
                            producerNameComboBox.currentText === Strings.empty ||
                            villageInputHeader.inputText === Strings.empty)

    function proceed() {
        pageManager.openPopup(Enums.Popup.WaitOverlay)

        var harvestDate = inputHarvestDate.currentDate
        var parcelCode = parcelCodesComboBox.currentText

        var properties = {
            [PackageDataProperties.ProducerId]: producerIdComboBox.currentText,
            [PackageDataProperties.ProducerName]: producerNameComboBox.currentText,
            [PackageDataProperties.Village]: villageInputHeader.inputText,
            [PackageDataProperties.ParcelCode]: parcelCode,
            [PackageDataProperties.HarvestDate]: harvestDate
        }

        dataManager.addAction(Enums.SupplyChainAction.Harvest,
                              harvestDate,
                              properties)
    }

    ListModel {
        id: parcelCodesModel

        function setItems(items) {
            clear()

            for (var item of items) {
                append({item})
            }

            if (items.length === 1) {
                parcelCodesComboBox.currentIndex = 0
            }
        }
    }

    pageContent: ColumnLayout {
        spacing: s(Style.smallMargin)

        Items.ComboBoxHeader {
            id: producerNameComboBox

            Layout.fillWidth: true

            headerText: Strings.nameOfProducer

            onActivated: {
                var producerId = ModelHelper.findItem(ModelHelper.roleNameToColumn("name", producersModel),
                                                      activatedText,
                                                      ModelHelper.roleNameToColumn("producerId", producersModel), producersModel)
                producerIdComboBox.selectItem(producerId)

                var village = ModelHelper.findItem(ModelHelper.roleNameToColumn("name", producersModel),
                                                   activatedText,
                                                   ModelHelper.roleNameToColumn("village", producersModel), producersModel)
                villageInputHeader.inputText = village

                var headers = ModelHelper.findItem(ModelHelper.roleNameToColumn("name", producersModel),
                                                   activatedText,
                                                   ModelHelper.roleNameToColumn("parcelCodes", producersModel), producersModel)
                parcelCodesModel.setItems(headers)
            }

            displayRole: "name"
            model: producersModel
        }

        Items.ComboBoxHeader {
            id: producerIdComboBox

            Layout.fillWidth: true

            headerText: Strings.producerIdNumber

            onActivated: {
                var producerName = ModelHelper.findItem(ModelHelper.roleNameToColumn("producerId", producersModel),
                                                        activatedText,
                                                        ModelHelper.roleNameToColumn("name", producersModel), producersModel)
                producerNameComboBox.selectItem(producerName)

                var village = ModelHelper.findItem(ModelHelper.roleNameToColumn("producerId", producersModel),
                                                   activatedText,
                                                   ModelHelper.roleNameToColumn("village", producersModel), producersModel)
                villageInputHeader.inputText = village

                var headers = ModelHelper.findItem(ModelHelper.roleNameToColumn("producerId", producersModel),
                                                   activatedText,
                                                   ModelHelper.roleNameToColumn("parcelCodes", producersModel), producersModel)
                parcelCodesModel.setItems(headers)
           }

           displayRole: "producerId"
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
            enabled: parcelCodesModel.count !== 0

            model: parcelCodesModel
        }

        Items.InputDateHeader {
            id: inputHarvestDate

            Layout.fillWidth: true

            headerText: Strings.harvestDate
        }
    }
}
