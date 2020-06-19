import QtQuick 2.11
import QtQuick.Controls 2.5

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

Popup
{
    id: top

    property int popup: Enums.Popup.InvalidPopup
    property string popupId

    anchors.centerIn: parent

    leftMargin: s(GStyle.hugeMargin)
    rightMargin: s(GStyle.hugeMargin)

    width: parent.width - leftMargin - rightMargin

    modal: true
    focus: true
    closePolicy: Popup.NoAutoClose

    Overlay.modal: Rectangle {
        color: GStyle.backgroundShadowColor
    }

    background: Rectangle {
        color: palette.window
        radius: s(GStyle.tinyMargin)
    }
}
