import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items
import "../headers" as Headers
import "../components" as Components
import "../pages" as Pages

Pages.SupplyChainPageBase {
    id: top

    title: Strings.summary

    proceedButtonEnabled: true
    proceedButtonText: Strings.confirm

    property bool isSummaryMode: (mainController.flavor === "cocoa")? false : true

    property var supplyChainPage
    property alias summary: summaryComponent.summary

    function proceed() {
        supplyChainPage.addAction()
    }

    Components.Summary {
        id: summaryComponent
        isSummaryMode: top.isSummaryMode
    }
}
