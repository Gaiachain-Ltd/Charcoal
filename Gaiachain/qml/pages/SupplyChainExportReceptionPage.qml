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

    title: Strings.receptionInCooperativeHQ

    property string scannedId

    proceedButtonEnabled: !(qrCodeInputHeader.inputText === Strings.empty ||
                            lotWeightInputHeader.inputText === Strings.empty)

    Component.onCompleted: refreshData()

    function refreshData() {
    }

    function proceed() {
        showOverlay()

        var codeData = qrCodeInputHeader.inputText
        var lotWeight = lotWeightInputHeader.inputText

        var properties = {
            [PackageDataProperties.Weight]: lotWeight,
        }

        top.packageCodeData = codeData
        dataManager.addAction(Enums.SupplyChainAction.ExportReception,
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
            headerText: Strings.qrCode
            placeholderText: Strings.scanQrCodeFrom.arg(Strings.lot.toUpperCase())

            onClicked: pageManager.enter(Enums.Page.QRScanner, {
                                                 "title": title,
                                                 "backSupplyChainPage": page,
                                                 "popupText": Strings.scanQrCodeFrom.arg(Strings.lot.toUpperCase()) })
        }

        Items.InputHeader {
            id: lotWeightInputHeader

            Layout.fillWidth: true

            validator: IntValidator {}
            inputMethodHints: Qt.ImhDigitsOnly

            headerText: Strings.kg.arg(Strings.lotWeight)

            iconSource: Style.rightArrowImgUrl

            placeholderText: Strings.typeHere + "..."
        }
    }
}
