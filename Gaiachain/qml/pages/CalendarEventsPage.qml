import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items
import "../components" as Components

CalendarPageBase {
    id: top

    function backHandler() {
        // called from BasePage
        enterMonthPage()
        return false    // do not close application
    }

    function refreshData() {
        // called from BasePage
        dataManager.fetchRangeEvents(new Date(currentYear, currentMonth, 1),
                                     new Date(currentYear, currentMonth + 1, 0))
    }

    function updateModelRange() {
        // called from CalendarPageBase
        var from = new Date(currentYear, currentMonth, 1)
        var to = new Date(currentYear, currentMonth + 1, 0)

        calendarModel.setDateRange(from, to)
        dateEventsModel.setSingleDateRange(new Date(currentYear, currentMonth, currentDay))
        dataManager.fetchRangeEvents(from, to)
    }

    function enterMonthPage() {
        pageManager.enter(Enums.Page.Calendar, {
                              "currentDay": currentDay,
                              "currentMonth": currentMonth,
                              "currentYear": currentYear })
    }

    onMonthHeaderClicked: enterMonthPage()

    calendarWidgets: Components.CalendarWeekItem {
        Layout.fillWidth: true

        currentDay: top.currentDay
        currentMonth: top.currentMonth
        currentYear: top.currentYear

        onCurrentDayChanged: top.currentDay = currentDay
        onCurrentMonthChanged: top.currentMonth = currentMonth
        onCurrentYearChanged: top.currentYear = currentYear
    }

    ColumnLayout {
        Layout.topMargin: 0
        Layout.bottomMargin: 0
        Layout.margins: 1.5 * s(Style.hugeMargin)
        spacing: 1.5 * s(Style.smallMargin)

        Items.BasicText {
            Layout.fillWidth: true

            horizontalAlignment: Text.AlignLeft
            font.weight: Font.DemiBold
            font.pixelSize: s(Style.subtitlePixelSize)

            text: new Date(top.currentYear, top.currentMonth, top.currentDay).toLocaleDateString(Qt.locale(), 'MMMM dd (dddd)')
        }

        Components.EventsListView {
            Layout.topMargin: -parent.spacing + s(Style.middleMargin)
            Layout.fillWidth: true
            Layout.fillHeight: true

            viewModel: latestDateEventsModel
            displayDate: false
            displayLastItemSeparator: true

            onDelegateClicked:  {
                var packageData = dataManager.getPackageData(packageId)
                pageManager.enter(Enums.Page.PackageData, { "title": top.title, "packageData": packageData })
            }
        }

        Items.BasicCheckBox {
            function updateCooperativeOnlyFiltering() {
                cooperativeFilteringEvents.active = checked
            }

            Layout.fillWidth: true
            Layout.bottomMargin: s(Style.hugeMargin)

            visible: userManager.loggedIn

            text: Strings.onlyMyTransactions
            checked: true

            Component.onCompleted: updateCooperativeOnlyFiltering()
            onCheckedChanged: updateCooperativeOnlyFiltering()
        }
    }
}
