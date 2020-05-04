import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../../items" as Items
import "../components" as Components


GPage {
    id: top

    title: Strings.transactions

    function refreshData() {
        // called from GPage
        latestRangeTransactionsModel.clearRowCount()
    }

    function updateCooperativeOnlyFiltering(active) {
        transactionsModel.setCooperativeOnly(active)
        latestRangeTransactionsModel.clearRowCount()
    }

    function updateSearch() {
        transactionsModel.setKeyword(searchInput.text)
        latestRangeTransactionsModel.clearRowCount()
    }

    function updatePackageTypeFiltering(packageType, checked) {
        transactionsModel.setPackageTypeActive(packageType, checked)
        latestRangeTransactionsModel.clearRowCount()
    }

    Component.onDestruction: {
        transactionsModel.clear()
        latestRangeTransactionsModel.clearRowCount()
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

        Items.GCheckBox {
            checked: true
            visible: userManager.loggedIn

            text: Strings.onlyMyTransactions

            Component.onCompleted: top.updateCooperativeOnlyFiltering(checked)
            onCheckedChanged: top.updateCooperativeOnlyFiltering(checked)
        }

        RowLayout {
            spacing: s(GStyle.smallMargin)

            Items.GText {
                Layout.fillWidth: true

                font.bold: true

                horizontalAlignment: Text.AlignLeft

                text: Strings.recentTransactions
            }

            Items.PureImageButton {
                Layout.alignment: Qt.AlignVCenter
                Layout.preferredHeight: s(GStyle.buttonImageSmallHeight)

                source: GStyle.calendarBlackImgUrl

                onClicked: {
                    var today = new Date()
                    pageManager.enter(Enums.Page.Calendar, {
                                        "currentDay": today.getDate(),
                                        "currentMonth": today.getMonth(),
                                        "currentYear": today.getFullYear() })
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true

            Repeater {
                id: packagesFilteringRepeater
                model: packageModel

                function updatePackageTypeFiltering(packageType, checked) {
                    top.updatePackageTypeFiltering(packageType, checked)
                }

                Items.RectCheckBox {
                    Layout.fillWidth: true
                    Layout.preferredWidth: GStyle.none
                    Layout.preferredHeight: s(GStyle.hugeMargin * 2)

                    checked: true
                    checkedColor: packageColor

                    text: packageName

                    Component.onCompleted: transactionsModel.isPackageTypeActive(packageType)
                    onCheckedChanged: packagesFilteringRepeater.updatePackageTypeFiltering(packageType, checked)
                }
            }
        }

        Components.EventsListView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            viewModel: latestRangeTransactionsModel

            onDelegateClicked:  {
                pageManager.enter(Enums.Page.PackageData, { "title": top.title, "packageId": packageId, "packageType": DataGlobals.packageType(action) })
            }
        }
    }
}
