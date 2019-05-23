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
            console.warn("CalendarMonthItem: Invalid month provided!")
        }

        return Strings.january +"!" // Add "!" if invalid
    }

    function getCurrentIdText() {
        switch(userManager.commodityType) {
        case Enums.CommodityType.Cocoa:
            return Strings.cocoaID
        case Enums.CommodityType.Timber:
            return Strings.logID
        default:
            return Strings.id
        }
    }

    function getCurrentCommodityTypeSuffix() {
        switch(userManager.commodityType) {
        case Enums.CommodityType.Cocoa:
            return "_brown"
        case Enums.CommodityType.Timber:
            return "_green"
        default:
            return ""
        }
    }

    function placeTypeToUrl(placeType) {
        switch(placeType) {
        case Enums.PlaceType.Forestery: return Style.timberImgUrl
        case Enums.PlaceType.LogPark: return Style.logParkImgUrl
        case Enums.PlaceType.Sawmill: return Style.sawmillImgUrl
        case Enums.PlaceType.Village: return Style.motorcycleImgUrl
        case Enums.PlaceType.Bagging: return Style.dryingImgUrl
        case Enums.PlaceType.Nursery: return Style.cocoaImgUrl
        case Enums.PlaceType.Truck: return Style.truckImgUrl
        case Enums.PlaceType.Export: return Style.exportImgUrl + getCurrentCommodityTypeSuffix()
        default:
            console.log("Wrong placeType:", placeType)
        }

        return ""
    }

    function placeTypeToString(placeType) {
        switch(placeType) {
        case Enums.PlaceType.Forestery: return Strings.placeTypeForestery
        case Enums.PlaceType.LogPark: return Strings.placeTypeLogPark
        case Enums.PlaceType.Sawmill: return Strings.placeTypeSawmill
        case Enums.PlaceType.Export: return Strings.placeTypeExport
        case Enums.PlaceType.Village: return Strings.placeTypeVillage
        case Enums.PlaceType.Bagging: return Strings.placeTypeBagging
        case Enums.PlaceType.Nursery: return Strings.placeTypeNursery
        case Enums.PlaceType.Truck: return Strings.placeTypeTruck
        default:
            console.log("Wrong placeType:", placeType)
        }

        return ""
    }

    function placeActionToString(placeAction) {
        switch(placeAction) {
        case Enums.PlaceAction.Arrived: return Strings.placeActionArrived
        case Enums.PlaceAction.Departed: return Strings.placeActionDeparted
        default:
            console.log("Wrong placeAction:", placeAction)
        }

        return ""
    }

    function placeActionToStringAction(placeAction) {
        switch(placeAction) {
        case Enums.PlaceAction.Arrived: return Strings.placeActionArrivedAction
        case Enums.PlaceAction.Departed: return Strings.placeActionDepartedAction
        default:
            console.log("Wrong placeAction:", placeAction)
        }

        return ""
    }

    function placeActionToDateTimeString(placeAction) {
        switch(placeAction) {
        case Enums.PlaceAction.Arrived: return Strings.dateTimeArrival
        case Enums.PlaceAction.Departed: return Strings.dateTimeDeparture
        default:
            console.log("Wrong placeAction:", placeAction)
        }

        return ""
    }
}
