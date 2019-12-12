import QtQuick 2.12
import QtQuick.Controls 2.12

import com.gaiachain.style 1.0

import "../items" as Items
import "../popups" as Popups

Popups.TimedPopup {
    id: top

    property string text

    anchors.centerIn: Overlay.overlay

    contentItem: Items.BasicText {
        color: Style.textSecondaryColor
        style: Text.Raised
        styleColor: Style.textSecondaryColor
        wrapMode: Text.WordWrap
        font {
            pixelSize: s(Style.popupPixelSize)
            weight: Font.DemiBold
        }

        opacity: Style.hidden
        text: top.text
    }

    background: Item {}

    Overlay.modal: Rectangle {
        color: Style.backgroundShadowColor

        Behavior on opacity {
            NumberAnimation { property: "opacity"; duration: Style.animationDuration; easing.type: Style.animationEasing }
        }
    }

    enter: Transition {
        PropertyAnimation { target: contentItem; property: "opacity"; to: Style.visible; duration: Style.animationDuration; easing.type: Style.animationEasing }
    }
}
