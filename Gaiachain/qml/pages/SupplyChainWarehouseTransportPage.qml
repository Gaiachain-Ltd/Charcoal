import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.types 1.0

import "../items" as Items
import "../components" as Components
import "../pages" as Pages

Pages.SupplyChainPage {
    id: top

    title: Strings.transportToCentralWarehouse

    property string scannedId

    validPageData: !(qrCodeInputHeader.inputText === Strings.empty ||
                     transporterComboBox.currentText === Strings.empty ||
                     destinationComboBox.currentText === Strings.empty)

    Component.onCompleted: refreshData()

    function refreshData() {
        sessionManager.getCompanies()
        sessionManager.getDestinations()
    }

    function proceed() {
        pageManager.enter(Enums.Page.SupplyChainSummary, { "supplyChainPage": this, "summary": summary(), "proceedButtonText": Strings.proceed })
    }

    function summary() {
        return [
            createSummaryItem(Strings.gpsCoordinates, gpsCoordinates, Style.gpsImgUrl),
            createSummaryItem(Strings.qrCode, qrCodeInputHeader.inputText),
            createSummaryItem(Strings.organicCocoaTransporter, transporterComboBox.currentText),
            createSummaryItem(Strings.destination, destinationComboBox.currentText),
            createSummaryItem(Strings.transportDate, inputDateHeader.selectedDate.toLocaleDateString(Qt.locale(), Strings.dateFormat))
        ]
    }

    function addAction() {
        showOverlay()

        var codeData = qrCodeInputHeader.inputText
        var transporterId = ModelHelper.getData(transporterComboBox.currentIndex, "id", transporterComboBox.model)
        var destinationId = ModelHelper.getData(destinationComboBox.currentIndex, "id", destinationComboBox.model)
        var transportDate = inputDateHeader.selectedDate

        var properties = {
            [PackageDataProperties.TransporterId]: transporterId,
            [PackageDataProperties.DestinationId]: destinationId,
            [PackageDataProperties.TransportDate]: transportDate
        }

        top.packageCodeData = codeData
        dataManager.addAction(Enums.SupplyChainAction.WarehouseTransport,
                              codeData,
                              coordinate(),
                              new Date,
                              properties)
    }

    Items.ButtonInputHeader {
        id: qrCodeInputHeader

        Layout.fillWidth: true

        iconSource: Style.qrImgUrl

        inputText: top.scannedId
        headerText: Strings.qrCode
        placeholderText: Strings.scanQrCodeFrom.arg(Strings.lot.toUpperCase())

        onClicked: pageManager.enter(Enums.Page.QRScanner, {
                                         "title": title,
                                         "backSupplyChainPage": page,
                                         "popupText": Strings.scanQrCodeFrom.arg(Strings.lot.toUpperCase()) })
    }

    Items.ComboBoxHeader {
        id: transporterComboBox

        Layout.fillWidth: true

        headerText: Strings.organicCocoaTransporter

        model: transportersModel
        displayRole: "name"
    }

    Items.ComboBoxHeader {
        id: destinationComboBox

        Layout.fillWidth: true

        headerText: Strings.destination

        model: destinationsModel
        displayRole: "name"
    }

    Items.InputDateHeader {
        id: inputDateHeader

        Layout.fillWidth: true

        headerText: Strings.transportDate
    }
}
