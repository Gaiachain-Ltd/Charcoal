import QtQuick 2.11

import com.gaiachain.enums 1.0

import "../items" as Items

BasePage {
    id: top

    function enterShipmentDetailsPage(data) {
        pageManager.push(Enums.Page.ShipmentDetails , data)
    }

    Items.EventsListView {
        anchors.fill: parent
        delegateHeight: s(100)

        onDelegateClicked: top.enterShipmentDetailsPage(data)

        viewModel: ListModel { //TO_DO
            ListElement {
                name: "Apple"
                cost: 2.45
            }
            ListElement {
                name: "Orange"
                cost: 3.25
            }
            ListElement {
                name: "Banana"
                cost: 1.95
            }
            ListElement {
                name: "Apple"
                cost: 2.45
            }
            ListElement {
                name: "Orange"
                cost: 3.25
            }
            ListElement {
                name: "Banana"
                cost: 1.95
            }
        }
    }
}
