import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

import "../items" as Items
import "../components" as Components

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

Items.GenericPanel
{
    id: top
    property alias headerVisible: header.visible
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
            refreshData()
        }
    }

    function refreshData() {
        // possibly redefined in each page
        sessionManager.ping()
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

    Connections {
        target: sessionManager
        enabled: (Number(pageManager.topPage) === page) && userManager.offlineMode

        onPingError: {
            pageManager.openPopup(Enums.Popup.Information,
                                  { "text": Strings.serverConnectionError })
        }
        onPingSuccess: {
            pageManager.openPopup(Enums.Popup.YesNoQuestion,
                                  { "text": Strings.onlineLogoutQuestion })
        }
    }
    Connections {
        target: pageManager
        enabled: (Number(pageManager.topPage) === page) && userManager.offlineMode

        onPopupAction: {
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
