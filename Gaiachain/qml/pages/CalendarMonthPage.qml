import QtQuick 2.11
import QtQuick.Layouts 1.11

import Qt.labs.calendar 1.0

import com.gaiachain.enums 1.0

import "../items" as Items

BasePage {
    id: top

    property int currentMonth: Calendar.December
    property int currentYear: 2018

    function enterCalendarEventsPage(date) {
        pageManager.enterPage(Enums.Page.CalendarEvents, {"currentDate" : date})
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: s(40)
        spacing: s(100)

        Text {
            Layout.fillWidth: true
            text: "<- " + top.getMonthName(top.currentMonth) + " " + top.currentYear
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft

            font.bold: true
            font.pixelSize: s(50)

            MouseArea {
                anchors.fill: parent
                onClicked: pageManager.pop()
            }
        }

        Items.CalendarMonthItem {
            Layout.fillHeight: true
            Layout.fillWidth: true

            currentMonth: top.currentMonth
            currentYear: top.currentYear

            bottomSpacing: s(100)

            dayFont.pixelSize: s(35)

            onDateClicked: top.enterCalendarEventsPage(d)
        }
    }

}
