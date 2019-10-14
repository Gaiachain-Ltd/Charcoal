pragma Singleton

import QtQuick 2.11

QtObject {
    // general
    readonly property string gaiachain: qsTr("Gaiachain")

    readonly property string ok: qsTr("Ok")
    readonly property string cancel: qsTr("Cancel")

    readonly property string supplyChainActionHarvest: qsTr("Harvest")
    readonly property string supplyChainActionFermentation: qsTr("Breaking, Fermenting")
    readonly property string supplyChainActionBagging: qsTr("Drying / Bagging")
    readonly property string supplyChainActionStorageArrival: qsTr("Storage")
    readonly property string supplyChainActionStorageLot: qsTr("Storage")
    readonly property string supplyChainActionTransport: qsTr("Transport")
    readonly property string supplyChainActionReception: qsTr("Reception")

    readonly property string harvestId: qsTr("Harvest ID")
    readonly property string sacId: qsTr("Sac ID")
    readonly property string lotId: qsTr("Lot ID")
    readonly property string arrivalId: qsTr("Arrival ID")

    readonly property string january: qsTr("January")
    readonly property string february: qsTr("February")
    readonly property string march: qsTr("March")
    readonly property string april: qsTr("April")
    readonly property string may: qsTr("May")
    readonly property string june: qsTr("June")
    readonly property string july: qsTr("July")
    readonly property string august: qsTr("August")
    readonly property string september: qsTr("September")
    readonly property string october: qsTr("October")
    readonly property string november: qsTr("November")
    readonly property string december: qsTr("December")

    // login
    readonly property string emailAddress: qsTr("Email address")
    readonly property string password: qsTr("Password")
    readonly property string login: qsTr("Login")
    readonly property string skipLogin: qsTr("Skip login")

    readonly property string loading: qsTr("Loading")
    readonly property string anonymousUser: qsTr("Anonymous")
    readonly property string searching: qsTr("Searching")
    readonly property string offline: qsTr("Offline")

    readonly property string noInternetError: qsTr("No internet connection.")
    readonly property string loginError: qsTr("Login failed. Try again.")

    // main menu
    readonly property string supplyChain: qsTr("Supply chain")
    readonly property string transactions: qsTr("Transactions")
    readonly property string calendar: qsTr("Calendar")
    readonly property string tracking: qsTr("Tracking")

    readonly property string supplyChainMenuCreateId: qsTr("(Create %1)")
    readonly property string supplyChainMenuHarvest: supplyChainActionHarvest + "\n" + supplyChainMenuCreateId.arg(harvestId.toUpperCase())
    readonly property string supplyChainMenuFermentation: supplyChainActionFermentation
    readonly property string supplyChainMenuBagging: supplyChainActionBagging + "\n" + supplyChainMenuCreateId.arg(sacId.toUpperCase())
    readonly property string supplyChainMenuStorageArrival: supplyChainActionStorageArrival + "\n" + supplyChainMenuCreateId.arg(arrivalId.toUpperCase())
    readonly property string supplyChainMenuStorageLot: supplyChainActionStorageLot + "\n" + supplyChainMenuCreateId.arg(lotId.toUpperCase())
    readonly property string supplyChainMenuTransport: supplyChainActionTransport
    readonly property string supplyChainMenuReception: supplyChainActionReception

    // calendar
    readonly property string activities: qsTr("activities")
}
