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
            if (localOnlyEventsModel.size) {
                dataManager.sendOfflineActions()
            }

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
        target: sessionManager
        enabled: (Number(pageManager.topPage) === page)

        function isSupplyChainPage() {
            return page === Enums.SupplyChainHarvest ||
                    page === Enums.SupplyChainGrainProcessing ||
                    page === Enums.SupplyChainSectionReception ||
                    page === Enums.SupplyChainBagging ||
                    page === Enums.SupplyChainAddHarvestId ||
                    page === Enums.SupplyChainLotCreation ||
                    page === Enums.SupplyChainWarehouseTransport ||
                    page === Enums.SupplyChainExportReception;
        }

        // handle notification for package sent error (here for offline added actions)
        onEntitySaveError: {
            if (isSupplyChainPage()
                    && isCurrentAction(packageId, codeData, action)) {
                return
            }

            // is offline added action
            if (RequestHelper.isNetworkError(code) || RequestHelper.isServerError(code)) {
                return
            }

            var errorText = Strings.addActionErrorUnknown
            if (RequestHelper.isActionMissingError(code)) {
                errorText = Strings.addActionErrorMissing
            } else if (RequestHelper.isActionDuplicatedError(code)) {
                errorText = Strings.addActionErrorDuplicated
            }

            errorText += "\n\n" + packageId
            errorText += "\n" + Helper.actionDescriptionStatusText(action)

            pageManager.openPopup(Enums.Popup.Information, {"text": errorText, "buttonPrimaryColor": Style.errorColor})
        }
    }

    Connections {
        target: pageManager
        enabled: (Number(pageManager.topPage) === page) && userManager.offlineMode
            && (page !== Enums.Login) && (page !== Enums.LoginLoading)

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
