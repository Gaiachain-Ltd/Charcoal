import QtQuick 2.11
import QtQuick.Controls 2.5

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items

ListView {
    signal actionClicked(int actionType)

    spacing: s(Style.normalMargin)
    interactive: false

    model: ListModel {
        ListElement {
            actionType: Enums.SupplyChainAction.Harvest
            packageType: Enums.PackageType.Harvest
            userType: Enums.UserType.Producer
        }
        ListElement {
            actionType: Enums.SupplyChainAction.Fermentation
            packageType: Enums.PackageType.Harvest
            userType: Enums.UserType.Producer
        }
        ListElement {
            actionType: Enums.SupplyChainAction.Bagging
            packageType: Enums.PackageType.Sac
            userType: Enums.UserType.BaggingPerson
        }
        ListElement {
            actionType: Enums.SupplyChainAction.StorageArrival
            packageType: Enums.PackageType.Sac
            userType: Enums.UserType.Storekeeper
        }
        ListElement {
            actionType: Enums.SupplyChainAction.StorageLot
            packageType: Enums.PackageType.Lot
            userType: Enums.UserType.Storekeeper
        }
        ListElement {
            actionType: Enums.SupplyChainAction.Transport
            packageType: Enums.PackageType.Lot
            userType: Enums.UserType.Exporter
        }
        ListElement {
            actionType: Enums.SupplyChainAction.Reception
            packageType: Enums.PackageType.Lot
            userType: Enums.UserType.Exporter
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
        padding: s(Style.normalMargin)
        spacing: s(Style.normalMargin)

        icon.height: s(Style.buttonImageSmallHeight)
        icon.width: s(Style.buttonImageSmallHeight)
        icon.source: Helpers.supplyChainActionIcon(actionType)

        width: ListView.view.width
        height: s(Style.submenuButtonHeight)

        font.pixelSize: s(Style.buttonPixelSize * 0.8)

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

            color: Helpers.packageTypeColor(packageType)
        }
    }
}
