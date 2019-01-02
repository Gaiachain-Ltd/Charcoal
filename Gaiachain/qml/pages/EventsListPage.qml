import QtQuick 2.11

import "../items" as Items

BasePage {
    Items.EventsListView {
        anchors.fill: parent
        anchors.margins: s(40)

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
