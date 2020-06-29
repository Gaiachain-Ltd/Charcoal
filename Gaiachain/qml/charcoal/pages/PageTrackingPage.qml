import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.types 1.0

import "../../items" as Items
import "../items" as CharcoalItems
import "../../common" as Common
import "../components" as CharcoalComponents
import "../../pages" as Pages

Pages.GPage {
    id: top

    title: Strings.recentTransactions

    Component.onCompleted: refreshData()

    function refreshData() {
        dataManager.trackingModel.refresh()
    }

    function updateSearch() {
        //transactionsModel.setKeyword(searchInput.text)
        //latestRangeTransactionsModel.clearRowCount()
    }

    Component.onDestruction: {
        //transactionsModel.clear()
        //latestRangeTransactionsModel.clearRowCount()
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

        ColumnLayout {
            Layout.fillWidth: true
            spacing: -bottomBar.height

            TabBar {
                id: bar
                Layout.fillWidth: true

                CharcoalItems.CharcoalTabButton {
                    text: Strings.allTab
                }

                CharcoalItems.CharcoalTabButton {
                    text: Strings.plotTab
                    selectedColor: GStyle.fontHighlightColor
                    packageTypes: Enums.PackageType.Plot
                }

                CharcoalItems.CharcoalTabButton {
                    text: Strings.harvestTab
                    selectedColor: GStyle.fontHighlightColor2
                    packageTypes: Enums.PackageType.Harvest
                }

                CharcoalItems.CharcoalTabButton {
                    text: Strings.transportTab
                    selectedColor: GStyle.fontHighlightColor4
                    packageTypes: Enums.PackageType.Transport
                }

            }

            Rectangle {
                id: bottomBar
                Layout.fillWidth: true

                height: sr(1)
                color: bar.currentItem.selectedColor
            }
        }

        CharcoalComponents.EventsListView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            model: dataManager.trackingModel

//                } else if (type === Enums.PackageType.Harvest) {
//                    summaryTitle = Strings.harvestIdDetails
//                    summary = [
//                        Utility.createSummaryItem(Strings.harvestId,
//                                          item.title,
//                                          "", "",
//                                          GStyle.delegateHighlightColor2,
//                                          GStyle.fontHighlightColor2,
//                                          "",
//                                          Enums.DelegateType.Standard),
//                        Utility.createSummaryItem("",
//                                          [
//                                              [ Strings.plotId ],
//                                              [ "AM003PM/0595112/04-03-2020" ],
//                                              [ GStyle.loupeUrl ],
//                                              [
//                                                  [
//                                                      Enums.Page.SupplyChainSummary,
//                                                      [
//                                                          "title", Strings.plotIdDetails,
//                                                          "proceedButtonVisible", false,
//                                                          "summary", plotSummary
//                                                      ]
//                                                  ]
//                                              ]
//                                          ],
//                                          "", "",
//                                          GStyle.delegateHighlightColor,
//                                          GStyle.fontHighlightColor,
//                                          "",
//                                          Enums.DelegateType.ColumnStack),
//                        // TODO: Collapsible delegate... ugh!
//                        Utility.createSummaryItem(Strings.beginningDate,
//                                          item.from),
//                        Utility.createSummaryItem(Strings.endingDate,
//                                          item.to),
//                        Utility.createSummaryItem(Strings.carbonizerId,
//                                          "AM004NA"),
//                        Utility.createSummaryItem(Strings.ovenDimensions,
//                                          [
//                                              [
//                                                  Strings.height,
//                                                  Strings.length,
//                                                  Strings.width
//                                              ],
//                                              [ 0.2, 12.0, 4.0 ]
//                                          ],
//                                          "", "",
//                                          "", "", "",
//                                          Enums.DelegateType.Row),
//                        Utility.createSummaryItem(Strings.timberVolume,
//                                          "12 m3"),

//                    ]


        }
    }
}
