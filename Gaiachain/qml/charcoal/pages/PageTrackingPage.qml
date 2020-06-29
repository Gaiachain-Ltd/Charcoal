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

            onDelegateClicked:  {
                let item
                for (let index = 0; index < dummyCharcoalModel.count; ++index) {
                    let current = dummyCharcoalModel.get(index)
                    if (current.type === type) {
                        item = current
                        break;
                    }
                }

                let summary = []
                let summaryTitle

                let plotSummary = [
                        Utility.createSummaryItem(Strings.plotId,
                                          item.title,
                                          "", "",
                                          GStyle.delegateHighlightColor,
                                          GStyle.fontHighlightColor,
                                          "",
                                          Enums.DelegateType.Standard),
                        Utility.createSummaryItem(Strings.malebiRepsId,
                                          "AM003PM"),
                        Utility.createSummaryItem(Strings.parcel,
                                          "0595112"),
                        Utility.createSummaryItem(Strings.village,
                                          "Wąchock"),
                        Utility.createSummaryItem(Strings.beginningDate,
                                          item.from),
                        Utility.createSummaryItem(Strings.endingDate,
                                          item.to),
                        Utility.createSummaryItem(Strings.treeSpecies,
                                          "Cassia siamea"),

                    ]

                if (item.type === Enums.PackageType.Plot) {
                    summaryTitle = Strings.plotIdDetails
                    summary = plotSummary
                } else if (item.type === Enums.PackageType.Harvest) {
                    summaryTitle = Strings.harvestIdDetails
                    summary = [
                        Utility.createSummaryItem(Strings.harvestId,
                                          item.title,
                                          "", "",
                                          GStyle.delegateHighlightColor2,
                                          GStyle.fontHighlightColor2,
                                          "",
                                          Enums.DelegateType.Standard),
                        Utility.createSummaryItem("",
                                          [
                                              [ Strings.plotId ],
                                              [ "AM003PM/0595112/04-03-2020" ],
                                              [ GStyle.loupeUrl ],
                                              [
                                                  [
                                                      Enums.Page.SupplyChainSummary,
                                                      [
                                                          "title", Strings.plotIdDetails,
                                                          "proceedButtonVisible", false,
                                                          "summary", plotSummary
                                                      ]
                                                  ]
                                              ]
                                          ],
                                          "", "",
                                          GStyle.delegateHighlightColor,
                                          GStyle.fontHighlightColor,
                                          "",
                                          Enums.DelegateType.ColumnStack),
                        // TODO: Collapsible delegate... ugh!
                        Utility.createSummaryItem(Strings.beginningDate,
                                          item.from),
                        Utility.createSummaryItem(Strings.endingDate,
                                          item.to),
                        Utility.createSummaryItem(Strings.carbonizerId,
                                          "AM004NA"),
                        Utility.createSummaryItem(Strings.ovenDimensions,
                                          [
                                              [
                                                  Strings.height,
                                                  Strings.length,
                                                  Strings.width
                                              ],
                                              [ 0.2, 12.0, 4.0 ]
                                          ],
                                          "", "",
                                          "", "", "",
                                          Enums.DelegateType.Row),
                        Utility.createSummaryItem(Strings.timberVolume,
                                          "12 m3"),

                    ]
                } else if (item.type === Enums.PackageType.Transport) {
                    let docs = picturesManager.documents()
                    let recs = picturesManager.receipts()
                    let hasDocs = docs.length > 0
                    let hasRecs = recs.length > 0
                    let docsIcon = "image://tickmark/document-" + hasDocs
                    let recsIcon = "image://tickmark/receipt-" + hasRecs

                    summaryTitle = Strings.transportIdDetails
                    summary = [
                        Utility.createSummaryItem(Strings.transportId,
                                          item.title,
                                          "", "",
                                          GStyle.delegateHighlightColor4,
                                          GStyle.fontHighlightColor4,
                                          "",
                                          Enums.DelegateType.Standard),
                        Utility.createSummaryItem("",
                                          [
                                              [
                                                  "150",
                                                  item.from,
                                                  hasDocs? Strings.uploaded : Strings.noPhoto,
                                                  hasRecs? Strings.uploaded : Strings.noPhoto
                                              ],
                                              [
                                                  Strings.numberOfBags,
                                                  Strings.loadingDate,
                                                  Strings.documents,
                                                  Strings.receipt
                                              ],
                                              [
                                                  "",
                                                  "",
                                                  docsIcon,
                                                  recsIcon
                                              ],
                                              [
                                                  "",
                                                  "",
                                                  [
                                                      hasDocs?
                                                          Enums.Page.PhotoGallery
                                                        : Enums.Page.InvalidPage,
                                                      [
                                                          "urls", docs
                                                      ]
                                                  ],
                                                  [
                                                      hasRecs?
                                                          Enums.Page.PhotoGallery
                                                        : Enums.Page.InvalidPage,
                                                      [
                                                          "urls", recs
                                                      ]
                                                  ]
                                              ]
                                          ],
                                          "", "",
                                          GStyle.delegateHighlightColor4,
                                          GStyle.fontHighlightColor4,
                                          GStyle.textPrimaryColor,
                                          Enums.DelegateType.ColumnStack),
                        Utility.createSummaryItem(Strings.plateNumber,
                                          "1234AB56"),
                        Utility.createSummaryItem(Strings.transportReceptionDate,
                                          item.to)

                    ]

                }

                pageManager.enter(Enums.Page.SupplyChainSummary,
                                  {
                                      "title": summaryTitle,
                                      "proceedButtonVisible": false,
                                      "summary": summary

                                      //"packageId": packageId,
                                      //"packageType": DataGlobals.packageType(action)
                                  })
            }
        }

        ListModel {
            id: dummyCharcoalModel

            Component.onCompleted: {
                append({
                           "title": "AM003PM/0595112/04-03-2020",
                           "type": Enums.PackageType.Plot,
                           "rows": 2,
                           "from": "04/03/2020",
                           "to": "07/03/2020",
                           "titleFrom": "Logging has begun",
                           "titleTo": "Logging has ended"
                       })

                append({
                           "title": "AM003PM/0595112/04-03-2020/AM004NA",
                           "type": Enums.PackageType.Harvest,
                           "rows": 2,
                           "from": "08/03/2020",
                           "to": "29/03/2020",
                           "titleFrom": "Oven A - carbonization has begun",
                           "titleTo": "Oven A - carbonization has ended"
                       })

                append({
                           "title": "AM003PM/0595112/04-03-2020/AM004NA/1234AB56/T1/31-03-2020",
                           "type": Enums.PackageType.Transport,
                           "rows": 2,
                           "from": "31/03/2020",
                           "to": "01/04/2020",
                           "titleFrom": "Bags have been loaded on truck 1234AB56",
                           "titleTo": Strings.transportReceptionDate
                       })
            }
        }
    }
}
