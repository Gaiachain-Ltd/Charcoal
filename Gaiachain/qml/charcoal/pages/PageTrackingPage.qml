import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.types 1.0

import "../items" as CharcoalItems
import "../../items" as Items
import "../../common" as Common
import "../../components" as Components
import "../../headers" as Headers
import "../headers" as CharcoalHeaders
import "../../pages" as Pages

Pages.GPage {
    id: top

    title: Strings.recentTransactions

    function refreshData() {
        // called from GPage
        //latestRangeTransactionsModel.clearRowCount()
    }

    ListModel {
        id: packageModel

        function initData() {
            var availablePackageTypes = DataGlobals.availablePackageTypes
            for (var idx = 0; idx < availablePackageTypes.length; ++idx) {
                var packageType = availablePackageTypes[idx]
                append({"packageType": packageType,
                           "packageColor": String(Helper.packageTypeColor(packageType)),
                           "packageName": Helper.packageTypeName(packageType)})
            }
        }

        Component.onCompleted: {
            initData()
        }
    }

    ColumnLayout {
        id: mainLayout

        spacing: s(GStyle.smallMargin)
        anchors {
            fill: parent
            leftMargin: s(GStyle.bigMargin)
            rightMargin: s(GStyle.bigMargin)
            topMargin: s(GStyle.bigMargin)
        }

        Items.GInput {
            id: searchInput
            Layout.fillWidth: true

            focus: false

            font.capitalization: text.length ? Font.AllUppercase : Font.MixedCase

            placeholderText: Strings.searchForTransaction
            iconSource: GStyle.searchImgUrl
            iconEdge: Enums.Edge.RightEdge

            onIconClicked: {
                focus = false
                accepted()
            }

            onFocusChanged: if (!focus) top.updateSearch()
            onAccepted: top.updateSearch()
        }

        TabBar {
            id: bar
            Layout.fillWidth: true

            background: Item {
                Rectangle {
                    anchors {
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                    }

                    height: sr(1)
                    color: GStyle.headerBackgroundColor
                }
            }

            TabButton {
                id: tab1
                property bool isCurrent: bar.currentItem === this
                text: Strings.allTab

                contentItem: Items.GText {
                    text: tab1.text
                    font.capitalization: Font.AllUppercase
                    color: tab1.isCurrent? GStyle.backgroundColor
                                         : GStyle.textTabColor
                }

                background: Rectangle {
                    radius: s(5)
                    border.color: GStyle.inputBorderColor
                    border.width: sr(1)
                    color: tab1.isCurrent? GStyle.headerBackgroundColor
                                         : GStyle.backgroundColor
                }
            }
            TabButton {
                id: tab2
                text: Strings.plotTab

                background: Rectangle {
                    radius: s(5)
                    border.color: GStyle.inputBorderColor
                    border.width: sr(1)
                    color: bar.currentIndex === tab2.index? GStyle.headerBackgroundColor
                                                     : GStyle.backgroundColor
                }
            }
            TabButton {
                text: Strings.harvestTab
            }
            TabButton {
                text: Strings.transportTab
            }
        }

        Components.EventsListView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            viewModel: latestRangeTransactionsModel

            onDelegateClicked:  {
                pageManager.enter(Enums.Page.PackageData,
                                  { "title": top.title,
                                      "packageId": packageId,
                                      "packageType": DataGlobals.packageType(action) }
                                  )
            }
        }
    }
}
