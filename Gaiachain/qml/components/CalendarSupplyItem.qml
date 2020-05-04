import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items
import "../components" as Components

Components.CalendarItem {
    id: top

    property date selectedDate

    currentMonth: selectedDate.getMonth()
    isToday: (selectedDate.getDate() === model.day &&
              selectedDate.getMonth() === model.month &&
              selectedDate.getFullYear() === model.year)

    dayNumberColor: top.isCurrentMonth ? (isToday ? GStyle.textSecondaryColor
                                                  : GStyle.textPrimaryColor)
                                       : GStyle.textDisabledColor

    implicitHeight: s(GStyle.calendarDayMinHeight)
    implicitWidth: s(GStyle.calendarDayMinHeight)

    todayIndicator {
        border.color: GStyle.backgroundColor
        color: GStyle.primaryColor
    }
}

