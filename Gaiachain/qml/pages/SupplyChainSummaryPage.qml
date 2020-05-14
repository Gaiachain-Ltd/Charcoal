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
        spacing: 0

        Repeater {
            model: summary

            delegate: Loader {
                Layout.fillWidth: true

                readonly property var value: summary[index].value
                readonly property int delegateType: summary[index].delegateType
                readonly property string headerValue: summary[index].headerValue
                readonly property string inputIconSource: summary[index].inputIconSource
                readonly property string suffixValue: summary[index].suffixValue
                readonly property string highlightColor: summary[index].highlightColor
                readonly property string decorationColor: summary[index].decorationColor
                readonly property string secondaryFontColor: summary[index].secondaryTextColor
                readonly property bool isHighlighted: summary[index].isHighlighted

                sourceComponent: {
                    if (delegateType === Pages.SupplyChainPageBase.Column) {
                        return columnComponent
                    } else if (delegateType === Pages.SupplyChainPageBase.ColumnStack) {
                        return columnStackComponent
                    } else if (delegateType === Pages.SupplyChainPageBase.Row) {
                        return rowComponent
                    } else if (delegateType === Pages.SupplyChainPageBase.Standard) {
                        return standardComponent
                    }

                    console.warn("No handling specified for delegate type:",
                                 delegateType)
                }
            }
        }
    }

    Component {
        id: columnComponent
        Headers.ColumnHeader {
            readonly property var composite: value
            Layout.fillWidth: true

            secondaryColor: isHighlighted? decorationColor
                                         : GStyle.textReadonlyColor
            secondaryTextColor: (secondaryFontColor.length === 0)? secondaryColor
                                                                 : secondaryFontColor
            backgroundColor: isHighlighted? highlightColor : GStyle.backgroundColor
            headerText: headerValue
            titles: composite[0]
            values: composite[1]
            icons: composite[2]
            readOnly: true
            summaryMode: isSummaryMode
            highlighted: isHighlighted
        }
    }

    Component {
        id: columnStackComponent
        Headers.ColumnStackHeader {
            readonly property var composite: value
            Layout.fillWidth: true

            secondaryColor: isHighlighted? decorationColor
                                         : GStyle.textReadonlyColor
            secondaryTextColor: (secondaryFontColor.length === 0)? secondaryColor
                                                                 : secondaryFontColor
            backgroundColor: isHighlighted? highlightColor : GStyle.backgroundColor
            headerText: headerValue
            titles: composite[0]
            values: composite[1]
            icons: composite[2]
            readOnly: true
            summaryMode: isSummaryMode
            highlighted: isHighlighted
        }
    }

    Component {
        id: rowComponent
        Headers.RowHeader {
            readonly property var composite: value
            Layout.fillWidth: true

            secondaryColor: isHighlighted? decorationColor
                                         : GStyle.textReadonlyColor
            secondaryTextColor: (secondaryFontColor.length === 0)? secondaryColor
                                                                 : secondaryFontColor
            backgroundColor: isHighlighted? highlightColor : GStyle.backgroundColor
            headerText: headerValue
            titles: composite[0]
            values: composite[1]
            icons: composite[2]
            readOnly: true
            summaryMode: isSummaryMode
            highlighted: isHighlighted
        }
    }

    Component {
        id: standardComponent

        Headers.InputHeader {
            Layout.fillWidth: true

            secondaryColor: isHighlighted? decorationColor
                                         : GStyle.textReadonlyColor
            secondaryTextColor: (secondaryFontColor.length === 0)? secondaryColor
                                                                 : secondaryFontColor
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
