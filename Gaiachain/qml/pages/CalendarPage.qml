import QtQuick 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

import "../items" as Items
import "../components" as Components

CalendarPageBase {
    id: top

    function updateModelRange() {
        // called from CalendarPageBase
        calendarModel.setDateRange(new Date(currentYear, currentMonth, 1),
                                   new Date(currentYear, currentMonth + 1, 0) )
    }

    function enterDayPage(dayDate) {
        pageManager.enter(Enums.Page.CalendarEvents, {
                              "currentDay": dayDate.getDate(),
                              "currentMonth": dayDate.getMonth(),
                              "currentYear": dayDate.getFullYear() })
    }

    ColumnLayout {
        spacing: s(Style.hugeMargin) * 3

        Components.CalendarPackageEventsBars {
            Layout.fillWidth: true
        }

        Items.BasicCheckBox {
            function updateCompanyOnlyFiltering() {
                companyCalendarModel.active = checked
            }

            Layout.fillWidth: true
            Layout.leftMargin: s(Style.hugeMargin) * 2
            Layout.bottomMargin: s(Style.hugeMargin)

            enabled: userManager.loggedIn
            opacity: userManager.loggedIn ? 1 : 0

            text: Strings.onlyMyTransactions

            Component.onCompleted: updateCompanyOnlyFiltering()
            onCheckedChanged: updateCompanyOnlyFiltering()
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

            onDayClicked: top.enterDayPage(dayDate)
        }
    }
}
