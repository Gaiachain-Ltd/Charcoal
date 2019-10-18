import QtQuick 2.11
import QtQuick.Controls 2.5

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items

ListView {
    signal actionClicked(int action)

    spacing: s(Style.middleBigMargin)
    interactive: false

    // TODO update with current solution
    model: ListModel {
        ListElement {
            actionType: Enums.SupplyChainAction.Harvest
            userType: Enums.UserType.SuperUser
        }
        ListElement {
            actionType: Enums.SupplyChainAction.GrainProcessing
            userType: Enums.UserType.SuperUser
        }
        ListElement {
            actionType: Enums.SupplyChainAction.SectionReception
            userType: Enums.UserType.SuperUser
        }
        ListElement {
            actionType: Enums.SupplyChainAction.Bagging
            userType: Enums.UserType.SuperUser
        }
        ListElement {
            actionType: Enums.SupplyChainAction.LotCreation
            userType: Enums.UserType.SuperUser
        }
        ListElement {
            actionType: Enums.SupplyChainAction.WarehouseTransport
            userType: Enums.UserType.SuperUser
        }
        ListElement {
            actionType: Enums.SupplyChainAction.ExportReception
            userType: Enums.UserType.SuperUser
        }
    }

    Component.onCompleted: {
        for (var index = model.count - 1; index >= 0; --index) {
            if (model.get(index).userType !== Number(userManager.userType)) {
                model.remove(index)
            }
        }
    }

    delegate: MenuButton {
        padding: s(Style.middleMargin)
        spacing: s(Style.middleMargin)

        icon.height: s(Style.submenuButtonImageHeight)
        icon.width: s(Style.submenuButtonImageHeight)
        icon.source: Helpers.supplyChainActionIcon(actionType)

        width: ListView.view.width
        height: s(Style.submenuButtonHeight)

        font.pixelSize: s(Style.submenuButtonPixelSize)

        text: Helpers.supplyChainActionMenuString(actionType)

        clip: true

        onClicked: ListView.view.actionClicked(actionType)

        Rectangle {
            anchors {
                top: parent.top
                topMargin: radius
                bottom: parent.bottom
                bottomMargin: radius

                right: parent.right
                rightMargin: -radius
            }

            radius: s(Style.buttonRadius)
            width: 2 * radius

            color: Helpers.packageTypeColor(DataGlobals.packageType(actionType))
        }
    }
}
