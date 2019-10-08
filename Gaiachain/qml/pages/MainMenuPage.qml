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

                visible: userManager.loggedIn
                spacing: 0

                Components.MenuButton {
                    Layout.fillWidth: true

                    text: Strings.supplyChain
                    icon.source: Style.supplyChainButtonImgUrl

                    onClicked: supplyChainSubmenu.menuVisible = !supplyChainSubmenu.menuVisible
                }

                Components.SupplyChainSubmenu {
                    id: supplyChainSubmenu
                    Layout.fillWidth: true

                    property bool menuVisible: false

                    opacity: 0
                    implicitHeight: 0

                    onMenuVisibleChanged: menuVisibleAnimation.start()

                    onActionClicked: console.log("SUPPLY CHAIN ACTION CHOOSEN:", actionType)    // TODO open pages

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
                            property: "implicitHeight"
                            from: supplyChainSubmenu.menuVisible ? supplyChainSubmenu.contentHeight : 0
                            to: supplyChainSubmenu.menuVisible ? 0 : supplyChainSubmenu.contentHeight
                            duration: 300
                        }
                        NumberAnimation {
                            target: supplyChainButtonColumn
                            property: "spacing"
                            from: supplyChainSubmenu.menuVisible ? menuColumn.spacing : 0
                            to: supplyChainSubmenu.menuVisible ? 0 : menuColumn.spacing
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

                onClicked: pageManager.enter(Enums.Page.Calendar)
            }
            Components.MenuButton {
                Layout.fillWidth: true

                text: Strings.tracking
                icon.source: Style.trackingButtonImgUrl

                onClicked: pageManager.enter(Enums.Page.Tracking)
            }
        }
    }
}
