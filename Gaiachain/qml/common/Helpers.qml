pragma Singleton

import QtQuick 2.11

import Qt.labs.calendar 1.0

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

QtObject {
    function convertTimestampToDate(timestamp) {
        var date = new Date(timestamp)
        return date.getFullYear() + "-" + formatDateValue(date.getMonth()+1) + "-" + date.getDate()
               + " " + formatDateValue(date.getHours()) + ":" + formatDateValue(date.getMinutes()) + ":" + formatDateValue(date.getSeconds())
    }

    function formatDateValue(val) {
        var formattedVal = val
        if (formattedVal < 10)
            formattedVal = "0" + formattedVal
        return formattedVal
    }

    function isNetworkError(code) {
        return code === 3 || code === 99
    }

    function formatLocation(location) {
        return location.lat.toFixed(6) + ", " + location.lon.toFixed(6)
    }

    function getMonthName(month) {
        switch(month) {
        case Calendar.January:return Strings.january
        case Calendar.February: return Strings.february
        case Calendar.March: return Strings.march
        case Calendar.April: return Strings.april
        case Calendar.May: return Strings.may
        case Calendar.June: return Strings.june
        case Calendar.July: return Strings.july
        case Calendar.August: return Strings.august
        case Calendar.September: return Strings.september
        case Calendar.October: return Strings.october
        case Calendar.November: return Strings.november
        case Calendar.December: return Strings.december
        default:
            console.warn("getMonthName: Invalid month provided!")
        }

        return Strings.january +"!" // Add "!" if invalid
    }
}
