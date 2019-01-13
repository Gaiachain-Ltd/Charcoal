import QtQuick 2.11

import com.gaiachain.enums 1.0

import "../items" as Items

BasePage {
    id: top

    function enterShipmentDetailsPage(data) {
        pageManager.enter(Enums.Page.ShipmentDetails , data)
    }

    Items.EventsListView {
        anchors.fill: parent
        delegateHeight: s(100)

        onDelegateClicked: top.enterShipmentDetailsPage(data)

        viewModel: latestEventsProxyModel
    }
}
