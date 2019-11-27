import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items
import "../components" as Components

Item {
    id: top

    readonly property int contentHeight: mainLayout.implicitHeight

    readonly property var actionsModel: {
        "": [Enums.SupplyChainAction.Harvest, Enums.SupplyChainAction.GrainProcessing,
             Enums.SupplyChainAction.Bagging, Enums.SupplyChainAction.LotCreation],
        "Transport": [Enums.SupplyChainAction.WarehouseTransport],
        "Reception": [Enums.SupplyChainAction.SectionReception, Enums.SupplyChainAction.ExportReception]
    }

    signal actionClicked(int action)

    implicitWidth: mainLayout.implicitWidth
    implicitHeight: mainLayout.implicitHeight

    function filterData(actions) {
        var availableActions = DataGlobals.userActionsQml(Number(userManager.userData.type))
        var filteredData = []

        for (var i = 0; i < actions.length; ++i) {
            if (availableActions.includes(actions[i])){
                filteredData.push(actions[i])
            }
        }

        return filteredData
    }

    function getFilteredActions(category) {
        var actions = filterData(actionsModel[category])
        var actionsDict = []

        for (var i = 0; i < actions.length; ++i) {
            actionsDict.push({ "actionName": String(Helper.supplyChainActionMenuString(actions[i])),
                               "actionColor": String(Helper.packageTypeColor(DataGlobals.packageType(actions[i]))),
                               "actionType": actions[i] })
        }

        return actionsDict
    }

    ListModel {
        id: nonCategoryActions

        Component.onCompleted: {
            append(getFilteredActions(Strings.empty))
        }
    }

    ListModel {
        id: transportActions

        Component.onCompleted: {
            append(getFilteredActions(Strings.transport))
        }
    }

    ListModel {
        id: receptionActions

        Component.onCompleted: {
            append(getFilteredActions(Strings.reception))
        }
    }

    ColumnLayout {
        id: mainLayout

        anchors.fill: parent
        spacing: s(Style.middleBigMargin)

        clip: true

        Repeater {
            model: nonCategoryActions

            Components.SupplyChainSubmenuButton {
                Layout.fillWidth: true
                onClicked: mainLayout.parent.actionClicked(actionType)
                color: actionColor
                text: actionName
            }
        }

        Items.LayoutSeparator {
            Layout.fillWidth: true

            visible: (transportExpandButton.visible && receptionExpandButton.visible)
        }

        Components.SupplyChainSubmenuExpandButton {
            id: transportExpandButton

            function closeOtherButtons() {
                if (opened) {
                    receptionExpandButton.opened = false
                }
            }

            Layout.fillWidth: true

            visible: (model.count !== 0)

            text: Strings.transport
            iconSource: Style.supplyChainSubmenuTransportImgUrl

            model: transportActions

            onClicked: mainLayout.parent.actionClicked(actionType)

            onOpenedChanged: closeOtherButtons()
        }

        Components.SupplyChainSubmenuExpandButton {
            id: receptionExpandButton

            function closeOtherButtons() {
                if (opened) {
                    transportExpandButton.opened = false
                }
            }

            Layout.fillWidth: true

            visible: (model.count !== 0)

            text: Strings.reception
            iconSource: Style.supplyChainSubmenuReceptionImgUrl

            model: receptionActions

            onClicked: mainLayout.parent.actionClicked(actionType)

            onOpenedChanged: closeOtherButtons()
        }
    }
}
