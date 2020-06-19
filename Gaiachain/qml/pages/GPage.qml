import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

import "../items" as Items
import "../components" as Components

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

Items.GPanel
{
    id: top
    property alias headerVisible: header.visible
    property alias showCloseButton: header.showCloseButton
    property alias footerVisible: footer.visible

    property alias logoVisible: header.logoVisible
    property alias backgroundColor: top.palette.window

    property bool errorDisplayed: false

    function closeEventHandler() {
        return backHandler() // calling back button
    }

    function backHandler() {
        pageManager.back()
        return false    // do not close application
    }

    function backToHomeHandler() {
        pageManager.backTo(pageManager.homePage())
    }

    function initialize() { // page beginning handler

    }

    function showOverlay(message = "") {
          pageManager.openPopup(Enums.Popup.WaitOverlay,
                                { "message" : message, })
      }

    function hideOverlay() {
        pageManager.closePopup()
    }

    function retryConnection() {
        if (userManager.offlineMode) {  // for offline always only ping
            sessionManager.ping()
        } else {
            if (localEventsModel.size) {
                dataManager.sendOfflineActions()
            }

            refreshData()
        }
    }

    function refreshData() {
        // possibly redefined in each page
        sessionManager.ping()
    }

    // TODO: use dedicated QtObject to hold these properties!
    function createSummaryItem(header, value, inputIconSource = "", suffix = "",
                               delegateType = SupplyChainPageBase.Standard,
                               highlightColor = "", decorationColor = "",
                               secondaryTextColor = "") {
        return {
            "headerValue": header,
            "value": value,
            "inputIconSource": inputIconSource.toString(),
            "suffixValue": suffix,
            "delegateType": delegateType,
            "highlightColor": Utility.colorString(highlightColor),
            "decorationColor": Utility.colorString(decorationColor),
            "secondaryTextColor": Utility.colorString(secondaryTextColor),
            "isHighlighted": (highlightColor !== "")
        }
    }

    header: Components.NavigationHeader {
        id: header
        Layout.fillWidth: true

        title: top.title
    }

    footer: Components.Footer {
        id: footer
        Layout.fillWidth: true
    }

    MouseArea {
        anchors.fill: parent
        onClicked: forceActiveFocus()
    }

    Components.OfflineModeHandler {}

    Connections {
        target: sessionManager
        enabled: (Number(pageManager.topPage) === page) && userManager.offlineMode

        onPingError: {
            pageManager.openPopup(Enums.Popup.Information,
                                  { "text": Strings.serverConnectionError })
        }
        onPingSuccess: {
            pageManager.openPopup(Enums.Popup.YesNoQuestion,
                                  { "text": Strings.onlineLogoutQuestion },
                                  "OFFLINE_LOGOUT")
        }
    }

    Connections {
        target: pageManager
        enabled: (Number(pageManager.topPage) === page) && userManager.offlineMode
            && (page !== Enums.Login) && (page !== Enums.LoginLoading)

        onPopupAction: {
            if (popupId != "OFFLINE_LOGOUT") {
                return
            }

            switch(action) {
            case Enums.PopupAction.Yes:
                header.logout()
                break
            default:
            }
        }
    }

    Connections {
        target: dbManager

        onProcessingChanged: {
            if (processing) {
                showOverlay(Strings.dbUpdateProgress)
            } else {
                hideOverlay()
            }
        }

        onDatabaseUpdateError: {
            pageManager.openPopup(Enums.Popup.Information,
                                  { "text": Strings.dbUpdateError })
        }
    }
}