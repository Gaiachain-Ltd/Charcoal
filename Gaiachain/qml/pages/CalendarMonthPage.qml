import QtQuick 2.11

import Qt.labs.calendar 1.0

import "../items" as Items

BasePage {
    id: top

    property int currentMonth: Calendar.December
    property int currentYear: 2018

    Items.CalendarMonthItem {
        anchors.fill: parent

        currentMonth: top.currentMonth
        currentYear: top.currentYear

        title: "<- " + getMonthName(currentMonth) + " " + currentYear
        titleClickable: true

        onTitleClicked: pageManager.pop()
    }
}
