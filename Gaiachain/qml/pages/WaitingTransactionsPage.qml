import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items
import "../components" as Components

BasePage {
    id: top

    title: Strings.waitingTransactions

    Components.EventsListView {
        anchors {
            fill: parent
            margins: s(Style.bigMargin)
        }

        viewModel: localOnlyEventsModel
    }
}
