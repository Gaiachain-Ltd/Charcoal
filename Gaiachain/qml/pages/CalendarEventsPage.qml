import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

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
        dataManager.fetchRangeEvents(getMonthStartDate(), getMonthEndDate())
        latestRangeDateModel.clearRowCount()
    }

    function updateCooperativeOnlyFiltering(active) {
        calendarMonthModel.setCooperativeOnly(active)
        calendarDateModel.setCooperativeOnly(active)
    }

    function updateModelRange() {
        // called from CalendarPageBase
        var from = getMonthStartDate()
        var to = getMonthEndDate()

        calendarMonthModel.setDateRange(from, to)
        dataManager.fetchRangeEvents(from, to)

        calendarDateModel.setDate(getTodayDate())
        latestRangeDateModel.clearRowCount()
    }

    function enterMonthPage() {
        pageManager.enter(Enums.Page.Calendar, {
                              "currentDay": currentDay,
                              "currentMonth": currentMonth,
                              "currentYear": currentYear })
    }

    onMonthHeaderClicked: enterMonthPage()

    // manual update required because binding is broken when user select a date in UI
    onCurrentDayChanged: calendarWeekItem.currentDay = currentDay

    calendarWidgets: Components.CalendarWeekItem {
        id: calendarWeekItem
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

            text: top.getTodayDate().toLocaleDateString(Qt.locale(), 'MMMM dd (dddd)')
        }

        Components.EventsListView {
            Layout.topMargin: -parent.spacing + s(Style.middleMargin)
            Layout.fillWidth: true
            Layout.fillHeight: true

            viewModel: latestRangeDateModel
            displayDate: false
            displayLastItemSeparator: true

            onDelegateClicked:  {
                pageManager.enter(Enums.Page.PackageData, { "title": top.title, "packageId": packageId, "packageType": DataGlobals.packageType(action) })
            }
        }

        Items.BasicCheckBox {
            id: onlyMyTransactionsCheckBox

            Layout.fillWidth: true
            Layout.bottomMargin: s(Style.hugeMargin)

            visible: userManager.loggedIn

            text: Strings.onlyMyTransactions
            checked: calendarMonthModel.cooperativeOnly

            Component.onCompleted: top.updateCooperativeOnlyFiltering(checked)
            onCheckedChanged: top.updateCooperativeOnlyFiltering(checked)
        }
    }
}
