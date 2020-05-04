import QtQuick 2.11
import QtQuick.Controls 2.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items

ItemDelegate {
    id: itemDelegate

    property bool bold: false
    property bool separatorVisible: false

    verticalPadding: s(GStyle.middleMargin)

    background: Rectangle {
        anchors.fill: parent

        color: itemDelegate.highlighted ? GStyle.delegateHighlightColor : GStyle.backgroundColor

        Rectangle {
            anchors.top: parent.top

            width: parent.width
            height: sr(GStyle.separatorHeight)

            visible: separatorVisible
            color: GStyle.separatorColor
        }
    }

    contentItem: Items.GText {
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft

        font.bold: itemDelegate.bold | itemDelegate.highlighted

        text: itemDelegate.text
    }
}
