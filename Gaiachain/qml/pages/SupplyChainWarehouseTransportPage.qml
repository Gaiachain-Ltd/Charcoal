import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.modelhelper 1.0
import com.gaiachain.packagedata 1.0

import "../items" as Items
import "../components" as Components
import "../pages" as Pages

Pages.SupplyChainPage {
    id: top

    title: Strings.transportToCentralWarehouse

    property string scannedId

    proceedButtonEnabled: !(qrCodeInputHeader.inputText === Strings.empty ||
                            transporterComboBox.currentText === Strings.empty ||
                            destinationComboBox.currentText === Strings.empty)

    Component.onCompleted: refreshData()

    function refreshData() {
        sessionManager.getCompanies()
        sessionManager.getDestinations()
    }

    function proceed() {
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
                              new Date,
                              properties)
    }

    pageContent: ColumnLayout {
        spacing: s(Style.smallMargin)

        Items.ButtonInputHeader {
            id: qrCodeInputHeader

            Layout.fillWidth: true

            iconSource: Style.qrImgUrl

            inputText: top.scannedId
            placeholderText: Strings.scanQrCodeFrom.arg(Strings.lot.toUpperCase())
            headerText: Strings.qrCode

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
}
