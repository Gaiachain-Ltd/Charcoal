import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items
import "../../headers" as Headers
import "../components" as Components
import "../pages" as Pages


Pages.SupplyChainPageBase {
    id: top

    title: Strings.summary

    proceedButtonEnabled: true
    proceedButtonText: Strings.confirm

    property var supplyChainPage
    property var summary

    onSummaryChanged: {
        dataSummaryListModel.clear()

        for (var summaryItem of summary) {
            dataSummaryListModel.append(summaryItem)
        }
    }

    function proceed() {
        supplyChainPage.addAction()
    }

    ListModel {
        id: dataSummaryListModel
    }

   ColumnLayout {
        spacing: s(GStyle.smallMargin)

        Repeater {
            model: dataSummaryListModel

            Headers.InputHeader {
                Layout.fillWidth: true

                color: GStyle.textReadonlyColor
                headerText: headerValue
                inputText: value
                suffixText: suffixValue
                readOnly: true
                iconSource: inputIconSource ? inputIconSource : ""
            }
        }
    }
}
