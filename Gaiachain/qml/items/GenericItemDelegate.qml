import QtQuick 2.11
import QtQuick.Controls 2.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items

ItemDelegate {
    id: itemDelegate

    property bool separatorVisible: false

    implicitWidth: indicatorInput.width
    implicitHeight: indicatorInput.implicitHeight

    background: Rectangle {
        anchors.fill: parent

        color: itemDelegate.highlighted ? Style.delegateHighlightColor : Style.backgroundColor

        Rectangle {
            anchors.top: parent.top

            width: parent.width
            height: sr(Style.separatorHeight)

            visible: separatorVisible
            color: Style.separatorColor
        }
    }

    contentItem: Items.BasicText {
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft

        font.bold: itemDelegate.highlighted

        text: itemDelegate.text
    }
}
