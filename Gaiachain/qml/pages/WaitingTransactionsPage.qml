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

    Connections {
        id: deleteConfirmConnections

        property string packageId
        property int action

        target: pageManager
        enabled: pageManager.isOnTop(page)
        onPopupAction: {
            if (popupId != "DELETE_CONFIRM") {
                return
            }

            switch (action) {
            case Enums.PopupAction.Yes:
                // !!! need to use deleteConfirmConnections, because bare action resolves to different variable
                dataManager.removeOfflineAction(deleteConfirmConnections.packageId,
                                                deleteConfirmConnections.action)
                break
            default:
                break
            }
        }
    }

    Components.EventsListView {
        anchors {
            fill: parent
            margins: s(Style.bigMargin)
        }

        delegateIcon: Style.deleteImgUrl
        viewModel: localEventsModel

        onDelegateIconClicked: {
            deleteConfirmConnections.packageId = packageId
            deleteConfirmConnections.action = action
            pageManager.openPopup(Enums.Popup.YesNoQuestion, { "text": Strings.askForActionDelete }, "DELETE_CONFIRM")
        }
    }
}
