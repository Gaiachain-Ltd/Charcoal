import QtQuick 2.12
import QtQuick.Controls 2.12

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

import "../items" as Items
import "../../popups" as Popups

Popups.TimedPopup {
    id: top

    property string text

    property color backgroundColor: GStyle.backgroundColor
    property color textColor: GStyle.textPrimaryColor

    anchors.centerIn: Overlay.overlay

    openedInterval: GStyle.commentPopupOpenedDefaultInterval

    contentItem: Items.GText {
        wrapMode: Text.WordWrap
        textFormat: Text.RichText
        padding: s(GStyle.bigMargin)
        color: top.textColor
        style: Text.Raised
        styleColor: color
        font.weight: Font.DemiBold

        text: top.text
    }

    enter: Transition {
        NumberAnimation { property: "opacity"; to: 1.0; duration: GStyle.animationDuration; easing.type: GStyle.animationEasing }
    }

    exit: Transition {
        NumberAnimation { property: "opacity"; to: 0.0; duration: GStyle.animationDuration; easing.type: GStyle.animationEasing }
    }

    Overlay.modal: Rectangle {
        color: GStyle.backgroundShadowColor

        Behavior on opacity {
            NumberAnimation { property: "opacity"; duration: GStyle.animationDuration; easing.type: GStyle.animationEasing }
        }
    }

    background: Rectangle {
        radius: s(GStyle.tinyMargin)
        color: top.backgroundColor

        MouseArea {
            anchors.fill: parent
            onClicked: top.forceClose()
        }
    }
}
