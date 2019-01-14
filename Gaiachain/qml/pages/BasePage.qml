import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

import "../items" as Items

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

Items.GenericPanel
{
    id: top
    property bool footerVisible: true
    property bool headerVisible: true

    property color backgroundColor: Style.pageBaseBackgroundColor

    property alias header: navigationHeader
    property alias mainOverlayVisible: mainOverlay.visible

    property alias refreshTimer: refreshDataTimer

    property bool errorDisplayed: false

    default property alias content: pageContent.data

    function closeEventHandler() {
        navigationHeader.backHandler() // calling back button
    }

    function refreshData() {
        mainOverlayVisible = true
        dataManager.clearModels()
        sessionManager.getEntity()
        refreshDataTimer.start()
    }

    Connections
    {
        target: sessionManager

        onEntityLoadError: {
            if (errorDisplayed || !pageManager.isOnTop(page))
                return
            errorDisplayed = true
            pageManager.enterPopup(Enums.Popup.Information, {
                                                            "text" : Strings.dataDownloadError,
                                                            "acceptButtonText": Strings.tryAgain,
                                                            "acceptButtonType": Enums.PopupAction.Error,
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
            case Enums.PopupAction.Error:
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

    ColumnLayout {
        id:  columnLayout
        anchors.fill: parent

        spacing: 0

        Items.NavigationHeader {
            id: navigationHeader
            Layout.fillWidth: true
            Layout.topMargin: mainWindow.headerTopMargin
            Layout.preferredHeight: s(Style.headerHeight)
            visible: top.headerVisible
            z: 5
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            layer.enabled: true
            clip: true

            Rectangle {
                id: background
                anchors.fill: parent
                color: top.backgroundColor
            }

            Item {
                id: pageContent
                readonly property int verticalOffset: -Math.max(mainWindow.bottomMargin, mainWindow.bottomMarginKeyboard * 0.5)
                anchors {
                    left: parent.left
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                    verticalCenterOffset: pageContent.verticalOffset
                }
                height: parent.height - mainWindow.bottomMarginKeyboard * 0.5
            }
        }

        Items.Footer {
            Layout.fillWidth: true
            // Math.round added because line below footer would appear
            Layout.preferredHeight: Math.round(s(Style.footerHeight))
            visible: top.footerVisible
        }
    }

    Items.WaitOverlay {
        id: mainOverlay
        anchors.fill: parent

        logoVisible: false
        color: Style.backgroundShadowColor
    }
}
