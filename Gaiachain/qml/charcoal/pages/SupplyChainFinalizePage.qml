import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.types 1.0

import "../../items" as Items
import "../../common" as Common
import "../../headers" as Headers
import "../headers" as CharcoalHeaders
import "../components" as CharcoalComponents
import "../../pages" as Pages

Pages.SupplyChainPageBase {
    id: top

    property string plotId
    property int scannedBagsCount: -1
    property int scannedBagsTotal: -1
    property int registeredTrucksCount: -1
    property int registeredTrucksTotal: -1

    readonly property int defaultMargin: s(GStyle.middleBigMargin)

    title: Strings.summary

    Component.onCompleted: refreshData()

    function refreshData() {
        // Nothing to do
    }

    function proceed() {
        addAction()
    }

    function summary() {
    }

    function addAction() {
        if (yes.selected) {
            dataManager.actionController.finalizeSupplyChain(plotId)
            pageManager.backTo(Enums.Page.MainMenu)
            return
        }

        pageManager.backTo(Enums.Page.SupplyChainReception)
    }

    Items.GText {
        Layout.fillWidth: true
        Layout.topMargin: defaultMargin
        Layout.bottomMargin: defaultMargin
        Layout.leftMargin: defaultMargin
        Layout.rightMargin: defaultMargin

        font.pixelSize: s(GStyle.subtitlePixelSize)
        font.capitalization: Font.AllUppercase
        font.bold: false
        horizontalAlignment: Text.AlignLeft
        wrapMode: Text.WordWrap
        text: Strings.doYouWantToCompleteSupplyChain
    }

    CharcoalComponents.SelectableListDelegate {
        Layout.fillWidth: true

        id: yes
        text: Strings.yes
        textLeftMargin: defaultMargin
        onClicked: {
            if (selected) {
               no.selected = false
            }
        }
    }

    CharcoalComponents.SelectableListDelegate {
        Layout.fillWidth: true

        id: no
        text: Strings.no
        textLeftMargin: defaultMargin
        selected: true
        onClicked: {
            if (selected) {
               yes.selected = false
            }
        }
    }

    Items.GText {
        Layout.fillWidth: true
        Layout.topMargin: 2 * defaultMargin
        Layout.leftMargin: defaultMargin
        Layout.rightMargin: defaultMargin

        font.pixelSize: s(GStyle.smallPixelSize)
        font.capitalization: Font.AllUppercase
        font.bold: true
        horizontalAlignment: Text.AlignLeft
        wrapMode: Text.WordWrap
        text: Strings.totalNumberOfScannedBags
    }

    Items.GText {
        Layout.fillWidth: true
        Layout.leftMargin: defaultMargin
        Layout.rightMargin: defaultMargin

        font.pixelSize: s(GStyle.smallPixelSize)
        horizontalAlignment: Text.AlignLeft
        text: Strings.numberOfBagsDetail.arg(scannedBagsCount).arg(scannedBagsTotal)
    }

    Items.GText {
        Layout.fillWidth: true
        Layout.topMargin: defaultMargin
        Layout.leftMargin: defaultMargin
        Layout.rightMargin: defaultMargin

        font.pixelSize: s(GStyle.smallPixelSize)
        font.capitalization: Font.AllUppercase
        font.bold: true
        horizontalAlignment: Text.AlignLeft
        wrapMode: Text.WordWrap
        text: Strings.totalNumberOfRegisteredTrucks
    }

    Items.GText {
        Layout.fillWidth: true
        Layout.leftMargin: defaultMargin
        Layout.rightMargin: defaultMargin

        font.pixelSize: s(GStyle.smallPixelSize)
        horizontalAlignment: Text.AlignLeft
        text: Strings.numberOfBagsDetail.arg(registeredTrucksCount).arg(registeredTrucksTotal)
    }
}
