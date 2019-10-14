pragma Singleton

import QtQuick 2.11

import Qt.labs.calendar 1.0

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

QtObject {
    function daysInMonth(year, month) {
        return new Date(year, month + 1, 0).getDate();
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

    function supplyChainActionString(action) {
        switch(action) {
        case Enums.SupplyChainAction.Harvest: return Strings.supplyChainActionHarvest
        case Enums.SupplyChainAction.Fermentation: return Strings.supplyChainActionFermentation
        case Enums.SupplyChainAction.Bagging: return Strings.supplyChainActionBagging
        case Enums.SupplyChainAction.StorageArrival: return Strings.supplyChainActionStorageArrival
        case Enums.SupplyChainAction.StorageLot: return Strings.supplyChainActionStorageLot
        case Enums.SupplyChainAction.Transport: return Strings.supplyChainActionTransport
        case Enums.SupplyChainAction.Reception: return Strings.supplyChainActionReception
        default:
            console.warn("supplyChainActionString: Invalid action provided!")
        }

        return ""
    }

    function supplyChainActionMenuString(action) {
        switch(action) {
        case Enums.SupplyChainAction.Harvest: return Strings.supplyChainMenuHarvest
        case Enums.SupplyChainAction.Fermentation: return Strings.supplyChainMenuFermentation
        case Enums.SupplyChainAction.Bagging: return Strings.supplyChainMenuBagging
        case Enums.SupplyChainAction.StorageArrival: return Strings.supplyChainMenuStorageArrival
        case Enums.SupplyChainAction.StorageLot: return Strings.supplyChainMenuStorageLot
        case Enums.SupplyChainAction.Transport: return Strings.supplyChainMenuTransport
        case Enums.SupplyChainAction.Reception: return Strings.supplyChainMenuReception
        default:
            console.warn("supplyChainActionString: Invalid action provided!", action)
        }

        return ""
    }

    function supplyChainActionIcon(action) {
        switch(action) {
        case Enums.SupplyChainAction.Harvest: return Style.supplyChainSubmenuHarvestImgUrl
        case Enums.SupplyChainAction.Fermentation: return Style.supplyChainSubmenuFermentationImgUrl
        case Enums.SupplyChainAction.Bagging: return Style.supplyChainSubmenuBaggingImgUrl
        case Enums.SupplyChainAction.StorageArrival: return Style.supplyChainSubmenuStorageArrivalImgUrl
        case Enums.SupplyChainAction.StorageLot: return Style.supplyChainSubmenuStorageLotImgUrl
        case Enums.SupplyChainAction.Transport: return Style.supplyChainSubmenuTransportImgUrl
        case Enums.SupplyChainAction.Reception: return Style.supplyChainSubmenuReceptionImgUrl
        default:
            console.warn("supplyChainActionIcon: Invalid action provided!", action)
        }

        return ""
    }


    function packageTypeName(packageType) {
        switch(packageType) {
        case Enums.PackageType.Harvest: return Strings.harvestId
        case Enums.PackageType.Sac: return Strings.sacId
        case Enums.PackageType.Lot: return Strings.lotId
        default:
            console.warn("packageTypeName: Invalid packageType provided!")
        }

        return Style.unknownColor
    }

    function packageTypeColor(packageType) {
        switch(packageType) {
        case Enums.PackageType.Harvest: return Style.harvestColor
        case Enums.PackageType.Sac: return Style.sacColor
        case Enums.PackageType.Lot: return Style.lotColor
        default:
            console.warn("packageTypeColor: Invalid packageType provided!")
        }

        return Style.unknownColor
    }
}
