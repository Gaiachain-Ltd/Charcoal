import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items
import "../components" as Components
import "../pages" as Pages

Pages.SupplyChainPageBase {
    id: top

    property bool firstEntry: true

    title: Strings.addHarvestId

    proceedButtonText: Strings.addHarvestId
    validPageData: !(harvestIdComboBox.currentText === Strings.empty)

    Component.onCompleted: refreshData()

    function refreshData() {
        dataManager.fetchLastActionPackageEvents(Enums.SupplyChainAction.SectionReception)
        lastActionSectionReceptionModel.resetFilter()
    }

    function proceed() {
        if (firstEntry) {
            pageManager.openPopup(Enums.Popup.Comment, { "text": Strings.cocoaFromSeveralPlotsComment }, "COCOA_COMMENT")
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
            if (popupId != "COCOA_COMMENT") {
                return
            }

            processHarvestData()
        }
    }

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

        optional: true
        headerText: Strings.kg.arg(Strings.bagWeight) + " (" + Strings.notRequired + ")"
        headerTextColor: Style.notRequiredTextInputColor

        iconSource: Style.rightArrowImgUrl

        placeholderText: Strings.typeHere + "..."
    }
}
