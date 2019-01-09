import QtQuick 2.11
import QtQuick.Layouts 1.11

import Qt.labs.calendar 1.0

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

import "../items" as Items

BasePage {
    id: top

    property int currentMonth: Calendar.December
    property int currentYear: 2018

    function enterCalendarEventsPage(date) {
        pageManager.enter(Enums.Page.CalendarEvents, {"currentDateStr" : Qt.formatDate(date, utility.defaultDateFormat())})
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: s(40)
        spacing: s(100)

        Items.ImageItem {
            Layout.fillWidth: true

            buttonHeight: s(30)
            buttonWidth: s(30)
            imageUrl: Style.backBlackImgUrl

            text: top.getMonthName(top.currentMonth) + " " + top.currentYear
            textFont.bold: true
            textFont.pixelSize: s(50)

            MouseArea {
                anchors.fill: parent
                onClicked: pageManager.back()
            }
        }

        Items.CalendarMonthItem {
            Layout.fillHeight: true
            Layout.fillWidth: true

            currentMonth: top.currentMonth
            currentYear: top.currentYear

            bottomSpacing: s(120)

            dayFont.pixelSize: s(35)

            onDateClicked: {
                console.log("Current date", d)
                top.enterCalendarEventsPage(d)
            }
        }
    }

}
