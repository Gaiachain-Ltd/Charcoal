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

    function refreshData() {
        // called from GPage
        latestRangeTransactionsModel.clearRowCount()
    }

    function updateSearch() {
        transactionsModel.setKeyword(searchInput.text)
        latestRangeTransactionsModel.clearRowCount()
    }

    Component.onDestruction: {
        transactionsModel.clear()
        latestRangeTransactionsModel.clearRowCount()
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
                }

                CharcoalItems.CharcoalTabButton {
                    text: Strings.harvestTab
                    selectedColor: GStyle.fontHighlightColor2
                }

                CharcoalItems.CharcoalTabButton {
                    text: Strings.transportTab
                    selectedColor: GStyle.fontHighlightColor4
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

            model: dummyCharcoalModel

            onDelegateClicked:  {
                pageManager.enter(Enums.Page.PackageData,
                                  {
                                      "title": top.title,
                                      "packageId": packageId,
                                      "packageType": DataGlobals.packageType(action) }
                                  )
            }
        }

        ListModel {
            id: dummyCharcoalModel

            Component.onCompleted: {
                append({
                           "title": "AM003PM/0595112/04-03-2020",
                           "type": "plot",
                           "values": [
                               ["Logging has ended", "07/03/2020"],
                               ["Logging has begun", "04/03/2020"]
                           ]})

                append({
                           "title": "AM003PM/0595112/04-03-2020/AM004NA",
                           "type": "harvest",
                           "values": [
                               ["Oven A - carbonization has ended", "29/03/2020"],
                               ["Oven A - carbonization has begun", "08/03/2020"]
                           ]
                       })

                append({
                           "title": "AM003PM/0595112/04-03-2020/AM004NA/1234AB56/T1/31-03-2020",
                           "type": "transport",
                           "values": [
                               ["Bags have been loaded on truck 1234AB56", "31/03/2020"],
                               ["Reception at storage facility", "01/04/2020"]
                           ]
                       })
            }
        }
    }
}
