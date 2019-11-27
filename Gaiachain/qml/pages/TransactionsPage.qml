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

    function refreshData() {
        // called from BasePage
        dataManager.fetchRangeEvents(latestRangePackagesTypeSearchEventsModel.oldestEventDate(),
                                     new Date, searchEventsModel.keyword)
    }

    function updateSearch() {
        searchEventsModel.search(searchInput.text)
        latestRangePackagesTypeSearchEventsModel.clearRowCount()
    }

    function updatePackageTypeFiltering(packageType, checked) {
        packagesTypeSearchEventsModel.setPackageTypeFiltering(packageType, checked)
        latestRangePackagesTypeSearchEventsModel.clearRowCount()
    }

    Component.onDestruction: {
        searchEventsModel.clearSearch()
        packagesTypeSearchEventsModel.clearFiltering()
        latestRangePackagesTypeSearchEventsModel.clearRowCount()
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

        spacing: s(Style.smallMargin)
        anchors {
            fill: parent
            margins: s(Style.bigMargin)
        }

        Items.GenericInput {
            id: searchInput
            Layout.fillWidth: true

            focus: false
            placeholderText: Strings.searchForTransaction
            iconSource: Style.searchImgUrl
            iconEdge: Enums.Edge.RightEdge

            onIconClicked: top.updateSearch()
            onAccepted: top.updateSearch()
        }


        Items.BasicCheckBox {
            function updateCooperativeOnlyFiltering() {
                cooperativeFilteringEvents.active = checked
            }

            checked: true
            visible: userManager.loggedIn

            text: Strings.onlyMyTransactions

            Component.onCompleted: updateCooperativeOnlyFiltering()
            onCheckedChanged: updateCooperativeOnlyFiltering()
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
                    Layout.preferredWidth: Style.none
                    Layout.preferredHeight: s(Style.hugeMargin * 2)

                    checked: true
                    checkedColor: packageColor

                    text: packageName

                    Component.onCompleted: packagesFilteringRepeater.updatePackageTypeFiltering(packageType, checked)
                    onCheckedChanged: packagesFilteringRepeater.updatePackageTypeFiltering(packageType, checked)
                }
            }
        }

        Components.EventsListView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            viewModel: latestRangePackagesTypeSearchEventsModel

            onDelegateClicked:  {
                var packageData = dataManager.getPackageData(packageId)
                pageManager.enter(Enums.Page.PackageData, { "title": top.title, "packageData": packageData })
            }
        }
    }
}
