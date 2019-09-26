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
        return header.backHandler() // calling back button
    }

    function showOverlay() {
        mainOverlay.open()
    }
    function hideOverlay() {
        mainOverlay.close()
    }

    function refreshData() {
        showOverlay()
        dataManager.clearModels()
        sessionManager.getEntity()
        refreshDataTimer.start()
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

    Items.WaitOverlay {
        id: mainOverlay
    }

    // TODO wait for real server answer or error
    Timer {
        id: refreshDataTimer
        interval: Style.requestOverlayInterval
        onTriggered: hideOverlay()
    }

}
