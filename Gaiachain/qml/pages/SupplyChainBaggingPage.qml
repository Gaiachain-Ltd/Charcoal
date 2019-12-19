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

    title: Strings.supplyChainMenuActionBagging

    property var lastHarvestData
    property string scannedId

    validPageData: !(lotIdComboBox.currentText === Strings.empty ||
                     nameCooperativeInputHeader.inputText === Strings.empty ||
                     qrCodeInputHeader.inputText === Strings.empty ||
                     harvestPidsRepeater.empty())

    onLastHarvestDataChanged: {
        var harvestId = lastHarvestData["harvestId"]
        if (harvestPidsModel.idExists(harvestId)) {
            delayedNotificationTimer.start()
        } else {
            harvestPidsModel.addNewHarvestId(harvestId, lastHarvestData["weight"])
        }
    }

    Component.onCompleted: refreshData()

    function refreshData() {
        sessionManager.getUnusedLotIds()
    }

    function proceed() {
        showOverlay()

        var codeData = qrCodeInputHeader.inputText
        var lotId = lotIdComboBox.currentText
        var harvestWeights = harvestPidsModel.getWeights()

        var properties = {
            [PackageDataProperties.LotPid]: lotId,
            [PackageDataProperties.HarvestWeights]: harvestWeights
        }

        top.packageCodeData = codeData
        dataManager.addAction(Enums.SupplyChainAction.Bagging,
                              codeData,
                              coordinate(),
                              new Date,
                              properties)
    }

    Connections {
        target: sessionManager

        onUnusedLotIdCreated: sessionManager.getUnusedLotIds()
    }

    ListModel {
        id: harvestPidsModel

        function getWeights() {
            var data = []
            for (var i = 0; i < count; ++i) {
                data.push({
                              [PackageDataProperties.Pid]: get(i).harvestIdValue,
                              [PackageDataProperties.Weight]: get(i).weightValue ? get(i).weightValue : "0"
                          })
            }
            return data
        }

        function addNewHarvestId(harvestId, weight) {
            append({ "harvestIdValue": harvestId,
                       "weightValue": weight })
        }

        function idExists(id) {
            for (var i = 0; i < count; ++i) {
                if (get(i).harvestIdValue === id) {
                    return true
                }

                return false
            }
        }
    }

    Timer {
        id: delayedNotificationTimer

        interval: Style.animationDuration
        onTriggered: pageManager.openPopup(Enums.Popup.Notification, {
                                               "text": Strings.alreadyAdded.arg(lastHarvestData["harvestId"]),
                                               "backgroundColor": Style.warningColor })
    }

    Items.ComboBoxHeader {
        id: lotIdComboBox

        Layout.fillWidth: true

        headerText: Strings.lotIdTheSackBelongsTo

        displayRole: "packageId"
        model: unusedLotIdsModel

        footerVisible: true
        footerText: Strings.addLot.toUpperCase()
        onFooterClicked: sessionManager.postUnusedLotId()
    }

    Items.InputHeader {
        id: nameCooperativeInputHeader

        Layout.fillWidth: true

        readOnly: true
        color: Style.headerBackgroundColor

        headerText: Strings.nameCooperative
        inputText: userManager.userData.cooperativeName
    }

    Items.ButtonInputHeader {
        id: qrCodeInputHeader

        Layout.fillWidth: true

        iconSource: Style.qrImgUrl

        inputText: top.scannedId
        headerText: Strings.qrCode
        placeholderText: Strings.scanQrCodeFrom.arg(Strings.sac.toUpperCase())


        onClicked: pageManager.enter(Enums.Page.QRScanner, {
                                         "title": title,
                                         "backSupplyChainPage": page,
                                         "popupText": Strings.attachQr.arg(Strings.sac.toUpperCase()) })
    }

    Items.ButtonInputHeader {
        Layout.fillWidth: true

        headerText: Strings.harvestId
        inputText: Strings.addHarvestId

        iconSource: Style.plusImgUrl

        onClicked: pageManager.enter(Enums.Page.SupplyChainAddHarvestId, { "firstEntry": typeof(lastHarvestData) === "undefined" })
    }

    Repeater {
        id: harvestPidsRepeater

        model: harvestPidsModel

        function empty() {
            return (count === 0)
        }

        Items.InputHeader {
            Layout.fillWidth: true

            iconSource: Style.deleteImgUrl
            readOnly: true

            inputText: harvestIdValue
            suffixText: Strings.kg.arg((weightValue === Strings.empty) ? "-" : weightValue)

            onIconClicked: harvestPidsModel.remove(index)
        }
    }
}
