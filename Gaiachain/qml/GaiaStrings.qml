pragma Singleton

import QtQuick 2.11

QtObject {
    // general
    readonly property string gaiachain: qsTr("Gaiachain")

    readonly property string ok: qsTr("Ok")
    readonly property string cancel: qsTr("Cancel")

    readonly property string empty: ""

    readonly property string success: qsTr("Success")
    readonly property string failed: qsTr("Failed")

    readonly property string harvestId: qsTr("Harvest ID")
    readonly property string sacId: qsTr("Sac ID")
    readonly property string lotId: qsTr("Lot ID")

    readonly property string details: qsTr("%1 details")

    readonly property string kg: qsTr("%1 kg")

    readonly property string transport: qsTr("Transport")
    readonly property string reception: qsTr("Reception")

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

    readonly property string dbUpdateProgress: qsTr("Database updating. Please wait...")
    readonly property string dbUpdateError: qsTr("Database update failed. If the error repeats, try to reainstall the application.")

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

    // events action description
    readonly property string actionHarvestDescriptionStatus: qsTr("Harvest done | Harvest ID created")
    readonly property string actionGrainProcessingDescriptionStatus: qsTr("Breaking, fermentation and drying finished")
    readonly property string actionSectionReceptionDescriptionStatus: qsTr("Received at the section")
    readonly property string actionBaggingDescriptionStatus: qsTr("Bagging finished | Sac ID created")
    readonly property string actionLotCreationDescriptionStatus: qsTr("Lot ID created")
    readonly property string actionWarehouseTransportDescriptionStatus: qsTr("Transport sent to Central Warehouse")
    readonly property string actionExportReceptionDescriptionStatus: qsTr("Received in the Cooperative HQ")

    // transactions page
    readonly property string searchForTransaction: qsTr("Search for a transaction")
    readonly property string onlyMyTransactions: qsTr("Only my transactions")
    readonly property string recentTransactions: qsTr("Recent transactions")

    // supply chain pages
    readonly property string proceed: qsTr("Proceed")

    readonly property string toSelect: qsTr("To select")
    readonly property string askForExit: qsTr("Are you sure you want to exit?")

    // package data details pages
    readonly property string nameOfProducer: qsTr("Name of the producer")
    readonly property string producerIdNumber: qsTr("Producer ID number")
    readonly property string village: qsTr("Village")
    readonly property string parcelCode: qsTr("Parcel code")
    readonly property string harvestDate: qsTr("Harvest date")
    readonly property string breakingDate: qsTr("Braking date")

    readonly property string organicCocoaTransporter: qsTr("Organic cocoa transporter")
    readonly property string destination: qsTr("Destination")
    readonly property string dateOfTransport: qsTr("Date of transport")

    readonly property string selectedAutomatically: qsTr("[selected automatically]")
    readonly property string selectProducer: qsTr("[select producer]")

    // calendar
    readonly property string activities: qsTr("activities")

    // special
    readonly property string checkMark: "\u2713"
}
