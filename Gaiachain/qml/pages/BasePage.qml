import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

import "../items" as Items

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0

Items.GenericPanel
{
    id: top
    property alias headerVisible: header.visible
    property alias footerVisible: footer.visible

    property alias backgroundColor: top.palette.window

    property alias mainOverlayVisible: mainOverlay.visible
    property alias refreshTimer: refreshDataTimer

    property bool errorDisplayed: false

    function closeEventHandler() {
        navigationHeader.backHandler() // calling back button
    }

    function refreshData() {
        mainOverlayVisible = true
        dataManager.clearModels()
        sessionManager.getEntity()
        refreshDataTimer.start()
    }

    header: Items.NavigationHeader {
        id: header
        Layout.fillWidth: true
    }

    footer: Items.Footer {
        id: footer
        Layout.fillWidth: true
    }

    Connections
    {
        target: sessionManager

        onEntityLoadError: {
            if (errorDisplayed || !pageManager.isOnTop(page))
                return
            errorDisplayed = true
            pageManager.enterPopup(Enums.Popup.Information, {
                                                            "text" : Strings.dataDownloadError + (Helpers.isNetworkError(code) ? "\n" + Strings.noInternet : ""),
                                                            "acceptButtonText": Strings.tryAgain,
                                                            "acceptButtonType": Enums.PopupAction.ReloadData,
                                                            "rejectButtonText": Strings.close,
                                                            "rejectButtonType": Enums.PopupAction.Cancel
                                   }, true)
        }
    }

    Connections {
        target: pageManager
        // When using popup always add checking if I'm on top
        enabled: pageManager.isOnTop(page)
        onPopupAction: {
            errorDisplayed = false
            switch(action) {
            case Enums.PopupAction.ReloadData:
                mainOverlayVisible = true
                refreshDateDelayTimer.start()
                break
            default:
            }
        }
    }

    Timer {
        id: refreshDateDelayTimer
        interval: 1000
        onTriggered: refreshData()
    }

    Timer {
        id: refreshDataTimer
        interval: Style.requestOverlayInterval
        onTriggered: mainOverlayVisible = false
    }


    Items.WaitOverlay {
        id: mainOverlay
        anchors.fill: parent

        color: Style.backgroundShadowColor
    }
}
