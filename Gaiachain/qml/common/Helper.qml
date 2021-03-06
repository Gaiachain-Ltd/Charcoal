pragma Singleton

import QtQuick 2.11

import Qt.labs.calendar 1.0

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

QtObject {
    function formatDate(timestamp) {
        return (new Date(timestamp)).toLocaleDateString(Qt.locale(), Strings.dateFormat)
    }

    function daysInMonth(year, month) {
        return new Date(year, month + 1, 0).getDate();
    }

    function formatLocation(location) {
        return location.lat.toFixed(6) + ", " + location.lon.toFixed(6)
    }

    function formatShortName(str) {
        return str.charAt(0).toUpperCase() + str.slice(1).replace(".", Strings.empty)
    }

    function formatCoordinate(coord) {
        var items = coord.split(",")
        return (items.length === 3 ? items.slice(0, -1) : items).join(", ")
    }

    function minusIfNotDefined(str) {
         return str ? str : Strings.minus
     }

    function minusIfNotDefinedOrZero(value) {
        return value && value > 0 ? value : Strings.minus
    }

    function minusIfNotDate(date) {
        return date ? formatDate(date) : Strings.minus
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

    function supplyChainActionMenuString(action) {
        switch(action) {
        case Enums.SupplyChainAction.Harvest: return Strings.supplyChainMenuActionHarvest
        case Enums.SupplyChainAction.GrainProcessing: return Strings.supplyChainMenuActionGrainProcessing
        case Enums.SupplyChainAction.SectionReception: return Strings.supplyChainMenuActionSectionReception
        case Enums.SupplyChainAction.Bagging: return Strings.supplyChainMenuActionBagging
        case Enums.SupplyChainAction.LotCreation: return Strings.supplyChainMenuActionLotCreation
        case Enums.SupplyChainAction.WarehouseTransport: return Strings.supplyChainMenuActionWarehouseTransport
        case Enums.SupplyChainAction.ExportReception: return Strings.supplyChainMenuActionExportReception
        default:
            console.warn("supplyChainActionMenuString: Invalid action provided!", action)
        }

        return ""
    }

    function supplyChainActionIcon(action) {
        switch(action) {
        case Enums.SupplyChainAction.Harvest: return GStyle.supplyChainSubmenuHarvestImgUrl
        case Enums.SupplyChainAction.GrainProcessing: return GStyle.supplyChainSubmenuGrainProcessingImgUrl
        case Enums.SupplyChainAction.Bagging: return GStyle.supplyChainSubmenuBaggingImgUrl
        case Enums.SupplyChainAction.LotCreation: return GStyle.supplyChainSubmenuLotCreationImgUrl

        case Enums.SupplyChainAction.WarehouseTransport: return GStyle.supplyChainSubmenuTransportImgUrl
        case Enums.SupplyChainAction.SectionReception: return GStyle.supplyChainSubmenuReceptionImgUrl
        case Enums.SupplyChainAction.ExportReception: return GStyle.supplyChainSubmenuReceptionImgUrl
        default:
            console.warn("supplyChainActionIcon: Invalid action provided!", action)
        }

        return ""
    }

    function actionDescriptionStatusText(action) {
        switch(action) {
            case Enums.SupplyChainAction.Harvest: return Strings.actionHarvestDescriptionStatus
            case Enums.SupplyChainAction.GrainProcessing: return Strings.actionGrainProcessingDescriptionStatus
            case Enums.SupplyChainAction.SectionReception: return Strings.actionSectionReceptionDescriptionStatus
            case Enums.SupplyChainAction.Bagging:  return Strings.actionBaggingDescriptionStatus
            case Enums.SupplyChainAction.LotCreation:  return Strings.actionLotCreationDescriptionStatus
            case Enums.SupplyChainAction.WarehouseTransport:  return Strings.actionWarehouseTransportDescriptionStatus
            case Enums.SupplyChainAction.ExportReception:  return Strings.actionExportReceptionDescriptionStatus
            default:
                console.warn("actionDescriptionStatus: Invalid action provided!", action)

            return ""
        }
    }


    function packageTypeName(packageType) {
        switch(packageType) {
        case Enums.PackageType.Harvest: return Strings.harvestId
        case Enums.PackageType.Sac: return Strings.sacId
        case Enums.PackageType.Lot: return Strings.lotId
        default:
            console.warn("packageTypeName: Invalid packageType provided!")
        }

        return GStyle.unknownColor
    }

    function packageTypeColor(packageType) {
        switch(packageType) {
        case Enums.PackageType.Harvest: return GStyle.harvestColor
        case Enums.PackageType.Sac: return GStyle.sacColor
        case Enums.PackageType.Lot: return GStyle.lotColor
        default:
            console.warn("packageTypeColor: Invalid packageType provided!")
        }

        return GStyle.unknownColor
    }
}
