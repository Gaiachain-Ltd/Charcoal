import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items

BasePage {
    id: top

    title: Strings.gaiachain
    logoVisible: true

    Flickable {
        anchors.fill: parent

        contentHeight: menuColumn.implicitHeight

        ColumnLayout {
            id: menuColumn

            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                margins: s(Style.hugeMargin)
            }
            spacing: s(Style.bigMargin)

            Items.MenuButton {
                Layout.fillWidth: true

                text: Strings.supplyChain
                icon.source: Style.supplyChainButtonImgUrl
                visible: userManager.loggedIn

                onClicked: {} // TODO supply chain list
            }
            Items.MenuButton {
                Layout.fillWidth: true

                text: Strings.transactions
                icon.source: Style.transactionsButtonImgUrl

                onClicked: pageManager.enter(Enums.Page.Transactions)
            }
            Items.MenuButton {
                Layout.fillWidth: true

                text: Strings.calendar
                icon.source: Style.calendarButtonImgUrl

                onClicked: pageManager.enter(Enums.Page.Calendar)
            }
            Items.MenuButton {
                Layout.fillWidth: true

                text: Strings.tracking
                icon.source: Style.trackingButtonImgUrl

                onClicked: pageManager.enter(Enums.Page.Tracking)
            }
        }
    }
}
