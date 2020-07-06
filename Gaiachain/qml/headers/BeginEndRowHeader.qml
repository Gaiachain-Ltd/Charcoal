import QtQuick 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items
import "../headers" as Headers

Item {
    id: top

    Layout.fillWidth: true

    property color secondaryColor
    property color secondaryTextColor
    property color backgroundColor
    property var headerTexts
    property var inputTexts
    property bool summaryMode: true
    property bool highlighted: false

    height: row.height

    RowLayout {
        id: row

        Headers.InputHeader {
implicitWidth: top.width * .5
            secondaryColor: top.highlighted? top.secondaryColor
                                           : GStyle.separatorColor
            secondaryTextColor: (top.secondaryTextColor.length === 0)? top.secondaryColor
                                                                     : GStyle.textReadonlyColor
            backgroundColor: top.highlighted? top.backgroundColor : GStyle.backgroundColor
            headerText: headerTexts[0]
            inputText: inputTexts[0]
            readOnly: true
            iconSource: ""
            summaryMode: top.summaryMode
            highlighted: top.highlighted
        }

        Headers.InputHeader {
            implicitWidth: top.width * .5

            secondaryColor: top.highlighted? top.secondaryColor
                                           : GStyle.separatorColor
            secondaryTextColor: (top.secondaryTextColor.length === 0)? top.secondaryColor
                                                                     : GStyle.textReadonlyColor
            backgroundColor: top.highlighted? top.backgroundColor : GStyle.backgroundColor
            headerText: headerTexts[1]
            inputText: inputTexts[1]
            readOnly: true
            iconSource: ""
            summaryMode: top.summaryMode
            highlighted: top.highlighted
        }
    }
}
