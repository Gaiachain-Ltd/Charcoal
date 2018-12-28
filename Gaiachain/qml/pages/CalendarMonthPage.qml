import QtQuick 2.11

import "../items" as Items

Items.CalendarMonthItem {
    title: "<- " + getMonthName(currentMonth) + " " + currentYear
    titleClickable: true

    onTitleClicked: pageManager.pop()
}
