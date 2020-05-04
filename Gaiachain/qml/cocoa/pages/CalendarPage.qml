import QtQuick 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

import "../../items" as Items
import "../components" as Components

CalendarPageBase {
    id: top

    function refreshData() {
        // called from GPage
        dataManager.fetchRangeEvents(getMonthStartDate(), getMonthEndDate())
    }

    function updateCooperativeOnlyFiltering(active) {
        calendarMonthModel.setCooperativeOnly(active)
    }

    function updateModelRange() {
        // called from CalendarPageBase
        var from = getMonthStartDate()
        var to = getMonthEndDate()

        calendarMonthModel.setDateRange(from, to)
        dataManager.fetchRangeEvents(from, to)
    }

    function enterDayPage(dayDate) {
        pageManager.enter(Enums.Page.CalendarEvents, {
                              "currentDay": dayDate.getDate(),
                              "currentMonth": dayDate.getMonth(),
                              "currentYear": dayDate.getFullYear() })
    }

    function initialize() {
        onlyMyTransactionsCheckBox.checked = calendarMonthModel.cooperativeOnly
    }

    onMonthHeaderClicked: {
        pageManager.enter(Enums.Page.CalendarYear, {
                              "currentMonth": currentMonth,
                              "currentYear": currentYear })
    }

    ColumnLayout {
        spacing: s(GStyle.bigMargin) * 1.5

        Components.CalendarPackageEventsBars {
            Layout.fillWidth: true
        }

        Items.GCheckBox {
            id: onlyMyTransactionsCheckBox

            Layout.fillWidth: true
            Layout.leftMargin: s(GStyle.hugeMargin) * 2
            Layout.bottomMargin: s(GStyle.bigMargin) * 1.5

            enabled: userManager.loggedIn
            opacity: userManager.loggedIn ? 1 : 0

            text: Strings.onlyMyTransactions
            checked: true

            Component.onCompleted: top.updateCooperativeOnlyFiltering(checked)
            onCheckedChanged: top.updateCooperativeOnlyFiltering(checked)
        }
    }

    calendarWidgets: Flickable {
        id: flickable
        Layout.fillHeight: true
        Layout.fillWidth: true

        contentWidth: calendarMonthItem.width
        contentHeight: calendarMonthItem.height
        clip: true
        boundsBehavior: Flickable.StopAtBounds
        interactive: contentWidth > width || contentHeight > height

        Components.CalendarMonthItem {
            id: calendarMonthItem
            width: flickable.width < implicitWidth ? implicitWidth : flickable.width
            height: flickable.height < implicitHeight ? implicitHeight : flickable.height

            currentMonth: top.currentMonth
            currentYear: top.currentYear

            delegate: Components.CalendarPageItem {
                Components.CalendarItemGridExtension {
                    id: gridExtension

                    modelDate: model.date
                    modelDay: model.day
                    modelMonth: model.month
                    modelWeekNumber: model.weekNumber

                    gridItem: calendarMonthItem.gridItem
                }

                visible: !gridExtension.additionalRow
                currentMonth: calendarMonthItem.currentMonth
            }

            onDayClicked: top.enterDayPage(dayDate)
        }
    }
}
