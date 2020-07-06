import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.types 1.0

import "../items" as Items
import "../headers" as Headers
import "../components" as Components
import "../pages" as Pages

Pages.SupplyChainPage {
    id: top

    title: Strings.receptionInCooperativeHQ

    property string scannedId

    validPageData: !(qrCodeInputHeader.inputText === Strings.empty ||
                     lotWeightInputHeader.inputText === Strings.empty)

    Component.onCompleted: refreshData()

    function refreshData() {
    }

    function proceed() {
        pageManager.enter(Enums.Page.SupplyChainSummary, { "supplyChainPage": this, "summary": summary(), "proceedButtonText": Strings.proceed })
    }

    function summary() {
        return [
            Utility.createSummaryItem(Strings.gpsCoordinates, gpsCoordinates, GStyle.gpsImgUrl),
            Utility.createSummaryItem(Strings.qrCode, qrCodeInputHeader.inputText),
            Utility.createSummaryItem(Strings.kg.arg(Strings.lotWeight), Strings.kg.arg(lotWeightInputHeader.inputText))
        ]
    }

    function addAction() {
        showOverlay()

        var codeData = qrCodeInputHeader.inputText
        var lotWeight = lotWeightInputHeader.inputText

        var properties = {
            [PackageDataProperties.Weight]: lotWeight,
        }

        top.packageCodeData = codeData
        dataManager.addAction(Enums.SupplyChainAction.ExportReception,
                              codeData,
                              coordinate(),
                              new Date,
                              properties)
    }

    Headers.ButtonInputHeader {
        id: qrCodeInputHeader

        Layout.fillWidth: true

        iconSource: GStyle.qrImgUrl

        inputText: top.scannedId
        headerText: Strings.qrCode
        placeholderText: Strings.scanQrCodeFrom.arg(Strings.lot.toUpperCase())

        onClicked: pageManager.enter(Enums.Page.QRScanner, {
                                         "title": title,
                                         "backSupplyChainPage": page,
                                         "popupText": Strings.scanQrCodeFrom.arg(Strings.lot.toUpperCase()) })
    }

    Headers.InputHeader {
        id: lotWeightInputHeader

        Layout.fillWidth: true

        validator: IntValidator {}
        additionalInputMethodHints: Qt.ImhDigitsOnly

        headerText: Strings.kg.arg(Strings.lotWeight)

        iconSource: GStyle.rightArrowImgUrl

        placeholderText: Strings.typeHere + "..."
    }
}
