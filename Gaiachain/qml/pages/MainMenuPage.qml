import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items
import "../components" as Components

BasePage {
    id: top

    title: Strings.gaiachain
    logoVisible: true

    function closeEventHandler() {
        return true // android back button will close app
    }

    function supplyChainPageForAction(action) {
        switch (Number(action)) {
        case Enums.SupplyChainAction.Harvest:
            return Enums.Page.SupplyChainHarvest
        case Enums.SupplyChainAction.GrainProcessing:
            return Enums.Page.SupplyChainGrainProcessing
        case Enums.SupplyChainAction.Bagging:
            return Enums.Page.SupplyChainBagging
        case Enums.SupplyChainAction.LotCreation:
            return Enums.Page.SupplyChainLotCreation
        case Enums.SupplyChainAction.WarehouseTransport:
            return Enums.Page.SupplyChainWarehouseTransport
        case Enums.SupplyChainAction.SectionReception:
            return Enums.Page.SupplyChainSectionReception
        case Enums.SupplyChainAction.ExportReception:
            return Enums.Page.SupplyChainExportReception  
        default:
            console.warn("navigateSupplyChainPages: Invalid action provided!")
        }
        return Enums.Page.InvalidPage
    }

    function navigateSupplyChainPages(action) {
        var supplyChainPage = supplyChainPageForAction(action)
        if (supplyChainPage !== Number(Enums.Page.InvalidPage)) {
            pageManager.enter(supplyChainPage, { "action": action })
            return true
        }
        return false
    }

    Flickable {
        anchors.fill: parent

        contentHeight: menuColumn.implicitHeight + 2 * menuColumn.margins
        boundsBehavior: Flickable.StopAtBounds

        ColumnLayout {
            id: menuColumn
            property int margins: s(Style.hugeMargin)

            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                margins: margins
            }
            spacing: s(Style.middleBigMargin)

            ColumnLayout {
                id: supplyChainButtonColumn

                visible: userManager.loggedIn && supplyChainSubmenu.availableActionsCount
                spacing: 0

                Components.MenuButton {
                    Layout.fillWidth: true

                    text: Strings.supplyChain
                    icon.source: Style.supplyChainButtonImgUrl

                    onClicked: supplyChainSubmenu.menuVisible = !supplyChainSubmenu.menuVisible
                }

                Components.SupplyChainSubmenu {
                    id: supplyChainSubmenu

                    property real initialHeight: Style.none
                    property bool menuVisible: false

                    Layout.fillWidth: true
                    Layout.preferredHeight: initialHeight

                    opacity: Style.none
                    clip: true

                    onMenuVisibleChanged: menuVisibleAnimation.start()

                    onContentHeightChanged: {
                        if (menuVisible) {
                            initialHeight = contentHeight
                        }
                    }

                    onActionClicked: {
                        if (navigateSupplyChainPages(action)) {
                            supplyChainSubmenu.menuVisible = false
                        }
                    }

                    ParallelAnimation {
                        id: menuVisibleAnimation

                        NumberAnimation {
                            target: supplyChainSubmenu
                            property: "opacity";
                            from: supplyChainSubmenu.menuVisible ? 1 : 0
                            to: supplyChainSubmenu.menuVisible ? 0 : 1
                            duration: 300
                        }
                        NumberAnimation {
                            target: supplyChainSubmenu
                            property: "initialHeight"
                            from: supplyChainSubmenu.menuVisible ? supplyChainSubmenu.contentHeight : Style.none
                            to: supplyChainSubmenu.menuVisible ? Style.none : supplyChainSubmenu.contentHeight
                            duration: 300
                        }
                        NumberAnimation {
                            target: supplyChainButtonColumn
                            property: "spacing"
                            from: supplyChainSubmenu.menuVisible ? menuColumn.spacing : Style.none
                            to: supplyChainSubmenu.menuVisible ? Style.none : menuColumn.spacing
                            duration: 300
                        }
                    }
                }
            }

            Components.MenuButton {
                Layout.fillWidth: true

                text: Strings.transactions
                icon.source: Style.transactionsButtonImgUrl

                onClicked: pageManager.enter(Enums.Page.Transactions)
            }

            Components.MenuButton {
                Layout.fillWidth: true

                text: Strings.calendar
                icon.source: Style.calendarButtonImgUrl

                onClicked: {
                    var today = new Date()
                    pageManager.enter(Enums.Page.Calendar, {
                                          "currentDay": today.getDate(),
                                          "currentMonth": today.getMonth(),
                                          "currentYear": today.getFullYear() })
                }
            }
        }
    }
}
