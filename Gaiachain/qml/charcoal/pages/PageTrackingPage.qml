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
                        createSummaryItem(Strings.plotId,
                                          item.title,
                                          "", "",
                                          Pages.SupplyChainPageBase.Standard,
                                          GStyle.delegateHighlightColor,
                                          GStyle.fontHighlightColor),
                        createSummaryItem(Strings.malebiRepsId,
                                          "AM003PM"),
                        createSummaryItem(Strings.parcel,
                                          "0595112"),
                        createSummaryItem(Strings.village,
                                          "Wąchock"),
                        createSummaryItem(Strings.beginningDate,
                                          item.from),
                        createSummaryItem(Strings.endingDate,
                                          item.to),
                        createSummaryItem(Strings.treeSpecies,
                                          "Cassia siamea"),

                    ]

                if (item.type === Enums.PackageType.Plot) {
                    summaryTitle = Strings.plotIdDetails
                    summary = plotSummary
                } else if (item.type === Enums.PackageType.Harvest) {
                    summaryTitle = Strings.harvestIdDetails
                    summary = [
                        createSummaryItem(Strings.harvestId,
                                          item.title,
                                          "", "",
                                          Pages.SupplyChainPageBase.Standard,
                                          GStyle.delegateHighlightColor2,
                                          GStyle.fontHighlightColor2),
                        createSummaryItem("",
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
                                          Pages.SupplyChainPageBase.ColumnStack,
                                          GStyle.delegateHighlightColor,
                                          GStyle.fontHighlightColor),
                        // TODO: Collapsible delegate... ugh!
                        createSummaryItem(Strings.beginningDate,
                                          item.from),
                        createSummaryItem(Strings.endingDate,
                                          item.to),
                        createSummaryItem(Strings.carbonizerId,
                                          "AM004NA"),
                        createSummaryItem(Strings.ovenDimensions,
                                          [
                                              [
                                                  Strings.height,
                                                  Strings.length,
                                                  Strings.width
                                              ],
                                              [ 0.2, 12.0, 4.0 ]
                                          ],
                                          "", "",
                                          Pages.SupplyChainPageBase.Row),
                        createSummaryItem(Strings.timberVolume,
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
                        createSummaryItem(Strings.transportId,
                                          item.title,
                                          "", "",
                                          Pages.SupplyChainPageBase.Standard,
                                          GStyle.delegateHighlightColor4,
                                          GStyle.fontHighlightColor4),
                        createSummaryItem("",
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
                                          Pages.SupplyChainPageBase.ColumnStack,
                                          GStyle.delegateHighlightColor4,
                                          GStyle.fontHighlightColor4,
                                          GStyle.textPrimaryColor),
                        createSummaryItem(Strings.plateNumber,
                                          "1234AB56"),
                        createSummaryItem(Strings.transportReceptionDate,
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