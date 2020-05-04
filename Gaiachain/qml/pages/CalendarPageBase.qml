import QtQuick 2.12
import QtQuick.Layouts 1.11

import Qt.labs.calendar 1.0

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items
import "../components" as Components

GPage {
    id: top
    title: Strings.calendar

    default property alias additionalContent: topLayout.data
    property alias calendarWidgets: topLayout.data

    readonly property var monthModel: [
        Calendar.January, Calendar.February, Calendar.March,
        Calendar.April, Calendar.May, Calendar.June,
        Calendar.July, Calendar.August, Calendar.September,
        Calendar.October, Calendar.November, Calendar.December
    ]

    property int currentDay: 0
    property int currentMonth: -1
    property int currentYear: 0

    signal monthHeaderClicked()

    function previousMonth() {
        if (currentMonth === Calendar.January) {
            currentMonth = Calendar.December
            --currentYear
        } else {
            --currentMonth
        }
    }
    function nextMonth() {
        if (currentMonth === Calendar.December) {
            currentMonth = Calendar.January
            ++currentYear
        } else {
            ++currentMonth
        }
    }

    function returnToCurrentMonth() {
        var date = new Date

        var day = date.getDate()
        var month = date.getMonth()
        var year = date.getFullYear()

        currentDay = day
        currentYear = year
        currentMonth = monthModel[month]
    }

    function onDateChanged() {
        if (currentDay > 0 && currentMonth >= 0 && currentYear > 0) {
            updateModelRange()
        }
    }

    function getMonthStartDate() { return new Date(currentYear, currentMonth, 1) }
    function getMonthEndDate() { return new Date(currentYear, currentMonth + 1, 0) }
    function getTodayDate() { return new Date(currentYear, currentMonth, currentDay) }

    onCurrentDayChanged: onDateChanged()
    onCurrentMonthChanged: onDateChanged()
    onCurrentYearChanged: onDateChanged()

    ColumnLayout {
        id: topLayout
        anchors.fill: parent

        spacing: s(GStyle.bigMargin)

        ColumnLayout {
            Layout.margins: s(2 * GStyle.bigMargin)
            Layout.bottomMargin: 0

            spacing: s(GStyle.normalMargin)

            Components.CalendarNavigator {
                Layout.fillWidth: true

                currentMonth: top.currentMonth
                currentYear: top.currentYear

                onNext: nextMonth()
                onPrevious: previousMonth()
                onReturnToCurrent: returnToCurrentMonth()
                onHeaderClicked: top.monthHeaderClicked()
            }

            // calendar widgets goes here
        }

        // additional content goes here
    }
}
