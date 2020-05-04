import QtQuick 2.11

import com.gaiachain.style 1.0

Rectangle {
    width: s(GStyle.calendarDayMinHeight)
    height: s(GStyle.calendarDayMinHeight)

    radius: s(GStyle.tinyMargin)
    border {
        width: s(GStyle.calendarCurrentDayBorderWidth)
        color: GStyle.inputBorderColor
    }

    color: GStyle.blank
}
