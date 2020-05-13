import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items
import "../headers" as Headers
import "../components" as Components
import "../pages" as Pages


Pages.SupplyChainPageBase {
    id: top

    title: Strings.summary

    proceedButtonEnabled: true
    proceedButtonText: Strings.confirm

    property bool isSummaryMode: (mainController.flavor === "cocoa")? false : true

    property var supplyChainPage
    property var summary

    function proceed() {
        supplyChainPage.addAction()
    }

    ColumnLayout {
        spacing: s(GStyle.smallMargin)

        Repeater {
            model: summary

            delegate: Loader {
                Layout.fillWidth: true

                readonly property var value: summary[index].value
                readonly property string headerValue: summary[index].headerValue
                readonly property string inputIconSource: summary[index].inputIconSource
                readonly property string suffixValue: summary[index].suffixValue
                readonly property string highlightColor: summary[index].highlightColor
                readonly property string highlightSecondaryColor: summary[index].highlightSecondaryColor
                readonly property bool isHighlighted: summary[index].isHighlighted

                sourceComponent: {
                    if (typeof(value) === "object") {
                        return dimensionsComponent
                    } else {
                        return normalComponent
                    }
                }
            }
        }
    }

    Component {
        id: dimensionsComponent
        Headers.DimensionsHeader {
            Layout.fillWidth: true

            secondaryColor: isHighlighted? highlightSecondaryColor
                                         : GStyle.textReadonlyColor
            backgroundColor: isHighlighted? highlightColor : GStyle.backgroundColor
            headerText: headerValue
            heightText: value["a"]
            lengthText: value["b"]
            widthText: value["c"]
            readOnly: true
            summaryMode: isSummaryMode
            highlighted: isHighlighted
        }
    }

    Component {
        id: normalComponent

        Headers.InputHeader {
            Layout.fillWidth: true

            secondaryColor: isHighlighted? highlightSecondaryColor
                                         : GStyle.textReadonlyColor
            backgroundColor: isHighlighted? highlightColor : GStyle.backgroundColor
            headerText: headerValue
            inputText: value
            suffixText: suffixValue
            readOnly: true
            iconSource: inputIconSource ? inputIconSource : ""
            summaryMode: isSummaryMode
            highlighted: isHighlighted
        }
    }
}
