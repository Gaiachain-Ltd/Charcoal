import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items
import "../components" as Components


BasePage {
    id: top

    title: Strings.transactions

    ListModel {
        id: packageModel

        function initData() {
            var availablePackageTypes = calendarPackageTypesModel.availablePackageTypesQml()
            for (var idx = 0; idx < availablePackageTypes.length; ++idx) {
                var packageType = availablePackageTypes[idx]
                append({"packageColor": String(Helpers.packageTypeColor(packageType)),
                        "packageName": Helpers.packageTypeName(packageType)})
            }
        }

        Component.onCompleted: {
            initData()
        }
    }

    ColumnLayout {
        id: mainLayout

        spacing: s(Style.smallMargin)
        anchors {
            fill: parent
            margins: s(Style.bigMargin)
        }

        Items.GenericInput {
            Layout.fillWidth: true

            focus: false
            placeholderText: Strings.searchForTransaction
            iconSource: Style.searchImgUrl
            iconEdge: Enums.Edge.RightEdge
        }


        Items.BasicCheckBox {
            checked: true

            text: Strings.onlyMyTransactions
        }

        RowLayout {
            spacing: s(Style.smallMargin)

            Items.BasicText {
                Layout.fillWidth: true

                font.bold: true

                horizontalAlignment: Text.AlignLeft

                text: Strings.recentTransactions
            }

            Items.PureImageButton {
                Layout.alignment: Qt.AlignVCenter
                Layout.preferredHeight: s(Style.buttonImageSmallHeight)

                source: Style.calendarBlackImgUrl

                onClicked: pageManager.enter(Enums.Page.Calendar)
            }
        }

        RowLayout {
            Layout.fillWidth: true

            Repeater {
                model: packageModel

                Items.RectCheckBox {
                    Layout.fillWidth: true
                    Layout.preferredWidth: Style.none
                    Layout.preferredHeight: s(Style.hugeMargin * 2)

                    checked: true
                    checkedColor: packageColor

                    text: packageName
                }
            }
        }

        Components.EventsListView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            viewModel: latestEventsProxyModel
        }
    }
}
