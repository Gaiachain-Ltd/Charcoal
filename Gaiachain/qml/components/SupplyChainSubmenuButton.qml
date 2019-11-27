import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items

MenuButton {
    property alias color: categoryLabel.color

    Layout.fillWidth: true
    Layout.preferredHeight: s(Style.submenuButtonHeight)

    padding: s(Style.middleMargin)
    spacing: s(Style.middleMargin)

    icon.height: s(Style.submenuButtonImageHeight)
    icon.width: s(Style.submenuButtonImageHeight)
    icon.source: Helper.supplyChainActionIcon(actionType)

    font.pixelSize: s(Style.submenuButtonPixelSize)

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

        radius: s(Style.buttonRadius)
        width: 2 * radius
    }
}
