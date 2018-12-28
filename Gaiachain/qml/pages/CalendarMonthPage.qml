import QtQuick 2.11

import Qt.labs.calendar 1.0

import com.gaiachain.enums 1.0

import "../items" as Items

BasePage {
    id: top

    property int currentMonth: Calendar.December
    property int currentYear: 2018

    function enterCalendarEventsPage(date) {
        pageManager.enterPage(Enums.Page.CalendarEvents, {
                                  "currentDay" : date.getDay(),
                                  "currentMonth" : date.getMonth(),
                                  "currentYear" : date.getFullYear()
                              })
    }

    Items.CalendarMonthItem {
        anchors.fill: parent

        currentMonth: top.currentMonth
        currentYear: top.currentYear

        title: "<- " + getMonthName(currentMonth) + " " + currentYear
        titleClickable: true
        titleFont {
            pixelSize: s(50)
            bold: true
        }

        internalMargins: s(50)
        titleCalendarSpacing: s(100)
        bottomSpacing: s(100)

        dayFont.pixelSize: s(35)

        onTitleClicked: pageManager.pop()
        onDateClicked: top.enterCalendarEventsPage(d)
    }
}
