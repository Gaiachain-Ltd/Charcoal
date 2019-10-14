import QtQuick 2.11

import com.gaiachain.enums 1.0

import "../items" as Items
import "../components" as Components

BasePage {
    id: top

    function enterShipmentDetailsPage(data) {
        pageManager.enter(Enums.Page.ShipmentDetails , data)
    }

    Components.EventsListView {
        anchors.fill: parent
        delegateHeight: s(100)

        onDelegateClicked: top.enterShipmentDetailsPage(data)

        viewModel: latestEventsModel
    }
}
