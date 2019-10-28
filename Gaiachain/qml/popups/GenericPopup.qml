import QtQuick 2.11
import QtQuick.Controls 2.5

import com.gaiachain.style 1.0

Popup
{
    id: top

    anchors.centerIn: parent

    leftMargin: s(Style.hugeMargin)
    rightMargin: s(Style.hugeMargin)

    width: parent.width - leftMargin - rightMargin

    modal: true
    focus: true
    closePolicy: Popup.NoAutoClose

    Overlay.modal: Rectangle {
        color: Style.backgroundShadowColor
    }

    background: Rectangle {
        color: palette.window
        radius: s(Style.tinyMargin)
    }
}
