import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items
import "../components" as Components
import "../pages" as Pages

Pages.SupplyChainPage {
    id: top

    property bool firstEntry: true

    title: Strings.addHarvestId

    proceedButtonText: Strings.addHarvestId
    proceedButtonEnabled: !(harvestIdComboBox.currentText === Strings.empty)

    Component.onCompleted: refreshData()

    function refreshData() {
        dataManager.fetchLastActionPackageEvents(Enums.SupplyChainAction.SectionReception)
    }

    function proceed() {
        if (firstEntry) {
            pageManager.openPopup(Enums.Popup.Comment, { "text": Strings.cocoaFromSeveralPlotsComment })
        } else {
            processHarvestData()
        }
    }

    function processHarvestData() {
        var harvestId = harvestIdComboBox.currentText
        var weight = weightInputHeader.inputText

        var harvestData = {
            "harvestId": harvestId,
            "weight": weight
        }
        pageManager.backTo(Enums.Page.SupplyChainBagging, {"lastHarvestData": harvestData})
    }

    Connections {
        target: pageManager
        enabled: pageManager.isOnTop(page)
        onPopupClosed: {
            switch (popup) {
            case Enums.Popup.Comment:
                processHarvestData()
                break
            default:
                break
            }
        }
    }

    pageContent: ColumnLayout {
        spacing: s(Style.smallMargin)

        Items.ComboBoxHeader {
            id: harvestIdComboBox

            Layout.fillWidth: true

            displayRole: "packageId"
            model: lastActionSectionReceptionModel
        }

        Items.InputHeader {
            id: weightInputHeader

            Layout.fillWidth: true

            validator: IntValidator {}
            inputMethodHints: Qt.ImhDigitsOnly

            headerText: Strings.kg.arg(Strings.bagWeight) + " (" + Strings.notRequired + ")"
            headerTextColor: Style.notRequiredTextInputColor

            iconSource: Style.rightArrowImgUrl

            placeholderText: Strings.typeHere + "..."
        }
    }
}
