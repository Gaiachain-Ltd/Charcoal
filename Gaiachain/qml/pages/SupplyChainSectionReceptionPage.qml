import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.types 1.0

import "../items" as Items
import "../../headers" as Headers
import "../components" as Components
import "../pages" as Pages

Pages.SupplyChainPage {
    id: top

    title: Strings.receptionAtSection

    validPageData: !(harvestIdComboBox.currentText === Strings.empty)

    Component.onCompleted: refreshData()

    function refreshData() {
        dataManager.fetchLastActionPackageEvents(Enums.SupplyChainAction.GrainProcessing)
        lastActionGrainProcessingModel.resetFilter()

        sessionManager.getCompanies()
    }

    function proceed() {
        pageManager.enter(Enums.Page.SupplyChainSummary, { "supplyChainPage": this, "summary": summary() })
    }

    function summary() {
        return [
            createSummaryItem(Strings.gpsCoordinates, gpsCoordinates, GStyle.gpsImgUrl),
            createSummaryItem(Strings.harvestId, harvestIdComboBox.currentText),
            createSummaryItem(Strings.receptionDate, receptionInputDateHeader.selectedDate.toLocaleDateString(Qt.locale(), Strings.dateFormat)),
            createSummaryItem(Strings.transportDate + " (" + Strings.notRequired + ")", Helper.minusIfNotDate(transportInputDateHeader.selectedDate)),
            createSummaryItem(Strings.buyer + " (" + Strings.notRequired + ")", Helper.minusIfNotDefined(buyerComboBox.currentText))
        ]
    }

    function addAction() {
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
                              coordinate(),
                              new Date,
                              properties)
    }

    Headers.ComboBoxHeader {
        id: harvestIdComboBox

        Layout.fillWidth: true

        headerText: Strings.harvestId

        model: lastActionGrainProcessingModel
        displayRole: "packageId"
    }

    Headers.InputDateHeader {
        id: receptionInputDateHeader

        Layout.fillWidth: true

        headerText: Strings.receptionDate
    }

    Headers.InputDateHeader {
        id: transportInputDateHeader

        Layout.fillWidth: true

        optional: true

        placeholderText: Strings.toSelect
        headerText: Strings.transportDate + " (" + Strings.notRequired + ")"
        headerTextColor: GStyle.notRequiredTextInputColor
    }

    Headers.ComboBoxHeader {
        id: buyerComboBox

        Layout.fillWidth: true

        optional: true

        placeholderText: Strings.toSelect
        headerText: Strings.buyer + " (" + Strings.notRequired + ")"
        headerTextColor: GStyle.notRequiredTextInputColor

        model: buyersModel
        displayRole: "name"
    }
}
