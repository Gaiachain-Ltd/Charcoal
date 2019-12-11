import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.packagedata 1.0

import "../items" as Items
import "../components" as Components
import "../pages" as Pages

Pages.SupplyChainPage {
    id: top

    title: Strings.supplyChainMenuActionLotCreation

    property string scannedId

    proceedButtonEnabled: !(lotIdComboBox.currentText === Strings.empty ||
                            qrCodeInputHeader.inputText === Strings.empty)

    Component.onCompleted: refreshData()

    function refreshData() {
        sessionManager.getUnusedLotIds()
    }

    function proceed() {
        showOverlay()

        var lotId = lotIdComboBox.currentText
        var codeData = qrCodeInputHeader.inputText
        var notes = descriptionHeader.inputText

        var properties = {
            [PackageDataProperties.Notes]: notes
        }

        top.packageId = lotId
        dataManager.addAction(lotId,
                              Enums.SupplyChainAction.LotCreation,
                              codeData,
                              new Date,
                              properties)
    }

    pageContent: ColumnLayout {
        spacing: s(Style.smallMargin)

        Items.ComboBoxHeader {
            id: lotIdComboBox

            Layout.fillWidth: true

            headerText: Strings.lotIdForThePackage

            displayRole: "packageId"
            model: unusedLotIdsModel
        }

        Items.InputHeader {
            id: qrCodeInputHeader

            Layout.fillWidth: true

            iconSource: Style.qrImgUrl

            inputText: top.scannedId
            headerText: Strings.registerQrCodeToLotId
            placeholderText: Strings.scanQrCodeFrom.arg(Strings.lot.toUpperCase())

            onIconClicked: pageManager.enter(Enums.Page.QRScanner, {
                                                 "title": title,
                                                 "backSupplyChainPage": page,
                                                 "popupText": Strings.attachQr.arg(Strings.lot.toUpperCase()) })
        }

        Items.TextAreaHeader {
            id: descriptionHeader

            Layout.fillWidth: true

            placeholderText: Strings.typeHere
            headerText: Strings.description
        }
    }
}
