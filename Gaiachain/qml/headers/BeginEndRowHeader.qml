import QtQuick 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items
import "../headers" as Headers

Row {
    id: root

    Layout.fillWidth: true

    property color secondaryColor
    property color secondaryTextColor
    property color backgroundColor
    property var headerTexts
    property var inputTexts
    property bool summaryMode: true
    property bool highlighted: false
    property bool extraLeftMargin: false

    Headers.TextHeader {
        implicitWidth: root.width * .5
        secondaryColor: root.highlighted? root.secondaryColor
                                        : GStyle.separatorColor
        secondaryTextColor: (root.secondaryTextColor.length === 0)? root.secondaryColor
                                                                  : GStyle.textReadonlyColor
        backgroundColor: root.highlighted? root.backgroundColor : GStyle.backgroundColor
        headerText: headerTexts[0]
        text: inputTexts[0]
        summaryMode: root.summaryMode
        highlighted: root.highlighted
        extraLeftMargin: root.extraLeftMargin
    }

     Headers.TextHeader {
        implicitWidth: root.width * .5

        secondaryColor: root.highlighted? root.secondaryColor
                                        : GStyle.separatorColor
        secondaryTextColor: (root.secondaryTextColor.length === 0)? root.secondaryColor
                                                                  : GStyle.textReadonlyColor
        backgroundColor: root.highlighted? root.backgroundColor : GStyle.backgroundColor
        headerText: headerTexts[1]
        text: inputTexts[1]
        summaryMode: root.summaryMode
        highlighted: root.highlighted
        extraLeftMargin: root.extraLeftMargin
    }
}
