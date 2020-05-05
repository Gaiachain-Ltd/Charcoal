import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items

MenuButton {
    property alias color: categoryLabel.color

    Layout.fillWidth: true
    Layout.preferredHeight: s(GStyle.submenuButtonHeight)

    padding: s(GStyle.middleMargin)
    spacing: s(GStyle.middleMargin)

    icon.height: s(GStyle.submenuButtonImageHeight)
    icon.width: s(GStyle.submenuButtonImageHeight)
    icon.source: Helper.supplyChainActionIcon(actionType)

    font.pixelSize: s(GStyle.submenuButtonPixelSize)

    clip: true

    Rectangle {
        id: categoryLabel

        anchors {
            top: parent.top
            topMargin: radius
            bottom: parent.bottom
            bottomMargin: radius

            right: parent.right
            rightMargin: -radius
        }

        radius: s(GStyle.buttonRadius)
        width: 2 * radius
    }
}
