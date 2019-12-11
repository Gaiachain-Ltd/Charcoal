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

    title: Strings.receptionAtSection

    proceedButtonEnabled: !(harvestIdComboBox.currentText === Strings.empty)

    Component.onCompleted: refreshData()

    function refreshData() {
        dataManager.fetchLastActionPackageEvents(Enums.SupplyChainAction.GrainProcessing)
        lastActionGrainProcessingModel.resetFilter()

        sessionManager.getCompanies()
    }

    function proceed() {
        showOverlay()

        var harvestId = harvestIdComboBox.currentText
        var receptionDate = receptionInputDateHeader.selectedDate
        var transportDate = transportInputDateHeader.selectedDate
        var buyerId = ModelHelper.getData(buyerComboBox.currentIndex, "id", buyerComboBox.model)

        var properties = {
            [PackageDataProperties.ReceptionDate]: receptionDate,
        }
        if (!transportInputDateHeader.isEmpty) {
            properties[PackageDataProperties.TransportDate] = transportDate
        }
        if (!buyerComboBox.isEmpty) {
            properties[PackageDataProperties.Buyer] = buyerId
        }

        top.packageId = harvestId
        dataManager.addAction(harvestId,
                              Enums.SupplyChainAction.SectionReception,
                              new Date,
                              properties)
    }

    pageContent: ColumnLayout {
        spacing: s(Style.smallMargin)

        Items.ComboBoxHeader {
            id: harvestIdComboBox

            Layout.fillWidth: true

            headerText: Strings.harvestId

            model: lastActionGrainProcessingModel
            displayRole: "packageId"
        }

        Items.InputDateHeader {
            id: receptionInputDateHeader

            Layout.fillWidth: true

            headerText: Strings.receptionDate
        }

        Items.InputDateHeader {
            id: transportInputDateHeader

            Layout.fillWidth: true

            optional: true

            placeholderText: Strings.toSelect
            headerText: Strings.transportDate + " (" + Strings.notRequired + ")"
            headerTextColor: Style.notRequiredTextInputColor
        }

        Items.ComboBoxHeader {
            id: buyerComboBox

            Layout.fillWidth: true

            optional: true

            placeholderText: Strings.toSelect
            headerText: Strings.buyer + " (" + Strings.notRequired + ")"
            headerTextColor: Style.notRequiredTextInputColor

            model: buyersModel
            displayRole: "name"
        }
    }
}
