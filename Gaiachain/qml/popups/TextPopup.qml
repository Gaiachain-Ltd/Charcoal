import QtQuick 2.12
import QtQuick.Controls 2.12

import com.gaiachain.style 1.0

import "../items" as Items
import "../popups" as Popups

Popups.TimedPopup {
    id: top

    property string text

    anchors.centerIn: Overlay.overlay

    contentItem: Items.GText {
        color: GStyle.textSecondaryColor
        style: Text.Raised
        styleColor: GStyle.textSecondaryColor
        wrapMode: Text.WordWrap
        font {
            pixelSize: s(GStyle.popupPixelSize)
            weight: Font.DemiBold
        }

        opacity: GStyle.hidden
        text: top.text
    }

    background: Item {}

    Overlay.modal: Rectangle {
        color: GStyle.backgroundShadowColor

        Behavior on opacity {
            NumberAnimation { property: "opacity"; duration: GStyle.animationDuration; easing.type: GStyle.animationEasing }
        }
    }

    enter: Transition {
        PropertyAnimation { target: contentItem; property: "opacity"; to: GStyle.visible; duration: GStyle.animationDuration; easing.type: GStyle.animationEasing }
    }
}
