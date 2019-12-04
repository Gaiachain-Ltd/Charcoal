import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.enums 1.0

import "../items" as Items

Item {
    id: top

    property alias viewModel: mainView.model
    property alias delegateHeight: mainView.delegateHeight

    property bool displayDate: true
    property bool displayLastItemSeparator: false

    function actionDescriptionStatusText(action) {
        switch(action) {
            case Enums.SupplyChainAction.Harvest: return Strings.actionHarvestDescriptionStatus
            case Enums.SupplyChainAction.GrainProcessing: return Strings.actionGrainProcessingDescriptionStatus
            case Enums.SupplyChainAction.SectionReception: return Strings.actionSectionReceptionDescriptionStatus
            case Enums.SupplyChainAction.Bagging:  return Strings.actionBaggingDescriptionStatus
            case Enums.SupplyChainAction.LotCreation:  return Strings.actionLotCreationDescriptionStatus
            case Enums.SupplyChainAction.WarehouseTransport:  return Strings.actionWarehouseTransportDescriptionStatus
            case Enums.SupplyChainAction.ExportReception:  return Strings.actionExportReceptionDescriptionStatus
            default:
                console.warn("actionDescriptionStatus: Invalid action provided!", action)

            return ""
        }
    }

    signal delegateClicked(string packageId, int action)

    ListView {
        id: mainView

        anchors.fill: parent

        property real delegateHeight: Style.listViewDelegateDefaultHeight

        spacing: s(Style.smallMargin)
        clip: true

        currentIndex: 0
        highlightRangeMode: ListView.StrictlyEnforceRange
        boundsBehavior: Flickable.StopAtBounds

        delegate: Item {
            id: delegate

            width: parent.width
            height: s(mainView.delegateHeight)

            readonly property bool isLast: ((ListView.view.count - 1) === index)

            ColumnLayout {
                anchors.fill: parent
                spacing: s(Style.tinyMargin)

                RowLayout {
                    spacing: s(Style.bigMargin)

                    Items.BasicText {
                        Layout.fillHeight: true

                        horizontalAlignment: Text.AlignLeft

                        font.bold: true
                        visible: displayDate

                        text: Helper.convertTimestampToDate(Number(timestamp))
                    }

                    Items.BasicText {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        horizontalAlignment: Text.AlignLeft

                        font.bold: true
                        color: Helper.packageTypeColor(DataGlobals.packageType(action))

                        text: packageId
                    }
                }

                Items.BasicText {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    horizontalAlignment: Text.AlignLeft

                    text: actionDescriptionStatusText(Number(action))
                }

                Items.LayoutSeparator {
                    Layout.fillWidth: true

                    visible: !delegate.isLast || displayLastItemSeparator
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: delegateClicked(packageId, action)
            }
        }
    }
}
