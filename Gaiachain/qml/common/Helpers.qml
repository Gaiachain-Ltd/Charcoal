pragma Singleton

import QtQuick 2.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

QtObject {
    // TO_DO move to Utility
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

    function formatLocation(location) {
        return location.lat.toFixed(6) + ", " + location.lon.toFixed(6)
    }

    function placeTypeToUrl(placeType) {
        switch(placeType) {
        case Enums.PlaceType.Forestery: return Style.timberImgUrl
        case Enums.PlaceType.LogPark: return Style.logParkImgUrl
        case Enums.PlaceType.Sawmill: return Style.sawmillImgUrl
        case Enums.PlaceType.Export: return Style.exportImgUrl
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