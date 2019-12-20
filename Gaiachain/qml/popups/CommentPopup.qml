import QtQuick 2.12
import QtQuick.Controls 2.12

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

import "../items" as Items
import "../popups" as Popups

Popups.TimedPopup {
    id: top

    property string text

    property color backgroundColor: Style.backgroundColor
    property color textColor: Style.textPrimaryColor

    anchors.centerIn: Overlay.overlay

    openedInterval: Style.commentPopupOpenedDefaultInterval

    contentItem: Items.BasicText {
        wrapMode: Text.WordWrap
        textFormat: Text.RichText
        padding: s(Style.bigMargin)
        color: top.textColor
        style: Text.Raised
        styleColor: color
        font.weight: Font.DemiBold

        text: top.text
    }

    enter: Transition {
        NumberAnimation { property: "opacity"; to: 1.0; duration: Style.animationDuration; easing.type: Style.animationEasing }
    }

    exit: Transition {
        NumberAnimation { property: "opacity"; to: 0.0; duration: Style.animationDuration; easing.type: Style.animationEasing }
    }

    Overlay.modal: Rectangle {
        color: Style.backgroundShadowColor

        Behavior on opacity {
            NumberAnimation { property: "opacity"; duration: Style.animationDuration; easing.type: Style.animationEasing }
        }
    }

    background: Rectangle {
        radius: s(Style.tinyMargin)
        color: top.backgroundColor

        MouseArea {
            anchors.fill: parent
            onClicked: top.forceClose()
        }
    }
}
