pragma Singleton

import QtQuick 2.11

QtObject {
    // general
    readonly property string gaiachain: qsTr("Gaiachain")

    readonly property string ok: qsTr("Ok")
    readonly property string cancel: qsTr("Cancel")

    readonly property string empty: ""

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
    readonly property string online: qsTr("Online")
    readonly property string offline: qsTr("Offline")

    readonly property string noInternetError: qsTr("No internet connection.")
    readonly property string loginError: qsTr("Login failed. Try again.")

    // main menu
    readonly property string supplyChain: qsTr("Supply chain")
    readonly property string transactions: qsTr("Transactions")
    readonly property string calendar: qsTr("Calendar")

    readonly property string supplyChainMenuActionHarvest: qsTr("Harvest")
    readonly property string supplyChainMenuActionGrainProcessing: qsTr("Breaking, Fermentation, Drying")
    readonly property string supplyChainMenuActionBagging: qsTr("Bagging")
    readonly property string supplyChainMenuActionLotCreation: qsTr("Lot creation")

    readonly property string supplyChainMenuActionTransport: qsTr("Transport")
    readonly property string supplyChainMenuActionWarehouseTransport: qsTr("to Central Warehouse")

    readonly property string supplyChainMenuActionReception: qsTr("Reception")
    readonly property string supplyChainMenuActionSectionReception: qsTr("at the Section")
    readonly property string supplyChainMenuActionExportReception: qsTr("in Cooperative HQ")

    // transactions page
    readonly property string searchForTransaction: qsTr("Search for a transaction")
    readonly property string onlyMyTransactions: qsTr("Only my transactions")
    readonly property string recentTransactions: qsTr("Recent transactions")

    // calendar
    readonly property string activities: qsTr("activities")

    // special
    readonly property string checkMark: "\u2713"
}
