import QtQuick 2.11

import com.gaiachain.style 1.0

Rectangle {
    width: s(Style.calendarDayMinHeight)
    height: s(Style.calendarDayMinHeight)

    radius: s(Style.tinyMargin)
    border {
        width: s(Style.calendarCurrentDayBorderWidth)
        color: Style.inputBorderColor
    }

    color: Style.blank
}
