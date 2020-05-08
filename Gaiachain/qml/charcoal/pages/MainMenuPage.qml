import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../../pages"
import "../../items" as Items
import "../components" as Components

GPage {
    id: top

    title: Strings.gaiachain
    logoVisible: true

    function closeEventHandler() {
        return true // android back button will close app
    }

    function supplyChainPageForAction(action) {
        switch (Number(action)) {
        case Enums.SupplyChainAction.LoggingBeginning:
            return Enums.Page.SupplyChainLoggingBeginning;
        case Enums.SupplyChainAction.LoggingEnding:
            return Enums.Page.SupplyChainLoggingEnding;
        case Enums.SupplyChainAction.CarbonizationBeginning:
            return Enums.Page.SupplyChainCarbonizationBeginning;
        case Enums.SupplyChainAction.CarbonizationEnding:
            return Enums.Page.SupplyChainCarbonizationEnding;
        case Enums.SupplyChainAction.LoadingAndTransport:
            return Enums.Page.SupplyChainLoadingAndTransport;
        case Enums.SupplyChainAction.Reception:
            return Enums.Page.SupplyChainReception;
        case Enums.SupplyChainAction.Tracking:
            return Enums.Page.SupplyChainTracking;
        case Enums.SupplyChainAction.Replantation:
            return Enums.Page.SupplyChainReplantation;
        default:
            console.warn("navigateSupplyChainPages: Invalid action provided!")
        }
        return Enums.Page.InvalidPage;
    }

    function navigateSupplyChainPages(action) {
        var supplyChainPage = supplyChainPageForAction(action)
        if (supplyChainPage !== Number(Enums.Page.InvalidPage)) {
            pageManager.enter(supplyChainPage, { "action": action })
            return true
        }
        return false
    }

    Component.onCompleted: {
        if (!userManager.offlineMode &&
                localEventsModel.size) {
            dataManager.sendOfflineActions()
        }

        sessionManager.getAdditionalData()
    }

    Flickable {
        anchors.fill: parent

        contentHeight: menuColumn.implicitHeight + 2 * menuColumn.margins
        boundsBehavior: Flickable.StopAtBounds

        ColumnLayout {
            id: menuColumn

            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }

            spacing: 0

                Components.ExpandableMenuButton {
                    Layout.fillWidth: true

                    text: Strings.supplyChain
                    icon: GStyle.menuSupplyChainUrl
                    color: GStyle.menuSupplyChainColor

                    onClicked: supplyChainSubmenu.visible = !supplyChainSubmenu.visible
                }

                ColumnLayout {
                    id: supplyChainSubmenu
                    spacing: 0
                    visible: false

                    Components.DoubleMenuButton {
                        Layout.fillWidth: true

                        text: Strings.logging
                        color: GStyle.submenuLoggingColor
                        icon: GStyle.submenuLoggingUrl
                        fontColor: GStyle.textPrimaryColor

                        leftText: Strings.loggingBeginning
                        leftColor: GStyle.submenuLoggingBeginningColor
                        leftIcon:  GStyle.submenuLoggingBeginningUrl
                        onLeftClicked: pageManager.enter(Enums.Page.LoggingBeginning)

                        rightText: Strings.loggingEnding
                        rightColor: GStyle.submenuLoggingEndingColor
                        rightIcon:  GStyle.submenuLoggingEndingUrl
                        onRightClicked: pageManager.enter(Enums.Page.LoggingEnding)
                    }

                    Components.DoubleMenuButton {
                        Layout.fillWidth: true

                        text: Strings.carbonization
                        color: GStyle.submenuCarbonizationColor
                        icon: GStyle.submenuCarbonizationUrl
                        fontColor: GStyle.textPrimaryColor

                        leftText: Strings.carbonizationBeginning
                        leftColor: GStyle.submenuCarbonizationBeginningColor
                        leftIcon:  GStyle.submenuCarbonizationBeginningUrl
                        onLeftClicked: pageManager.enter(Enums.Page.CarbonizationBeginning)

                        rightText: Strings.carbonizationEnding
                        rightColor: GStyle.submenuCarbonizationEndingColor
                        rightIcon:  GStyle.submenuCarbonizationEndingUrl
                        onRightClicked: pageManager.enter(Enums.Page.CarbonizationEnding)
                    }

                    Components.MenuButton {
                        Layout.fillWidth: true

                        text: Strings.loadingAndTransport
                        icon: GStyle.submenuLoadingAndTransportUrl
                        color: GStyle.submenuLoadingAndTransportColor
                        fontColor: GStyle.textPrimaryColor

                        onClicked: pageManager.enter(Enums.Page.LoadingAndTransport)
                    }

                    Components.MenuButton {
                        Layout.fillWidth: true

                        text: Strings.reception
                        icon: GStyle.submenuReceptionUrl
                        color: GStyle.submenuReceptionColor
                        fontColor: GStyle.textPrimaryColor

                        onClicked: pageManager.enter(Enums.Page.Reception)
                    }
                }

            Components.MenuButton {
                Layout.fillWidth: true

                text: Strings.tracking
                icon: GStyle.menuTrackingUrl
                color: GStyle.menuTrackingColor

                onClicked: pageManager.enter(Enums.Page.Tracking)
            }

            Components.MenuButton {
                Layout.fillWidth: true

                text: Strings.replantation
                icon: GStyle.menuReplantationUrl
                color: GStyle.menuReplantationColor

                onClicked: pageManager.enter(Enums.Page.Replantation)
            }
        }
    }
}

