import QtQuick 2.12
import QtQuick.Controls 2.12

import com.gaiachain.style 1.0

import "../items" as Items
import "../popups" as Popups

Popups.TimedPopup {
    id: top

    property color backgroundColor: Style.primaryColor

    property string text

    readonly property int topMarginStart: (parent.height - height - s(Style.hugeMargin) * 6 - s(Style.transitionMoveYRange))
    readonly property int topMarginFinish: (parent.height - height - s(Style.hugeMargin) * 6)

    openedInterval: Style.notificationPopupOpenedDefaultInterval

    topMargin: topMarginStart
    padding: s(Style.hugeMargin)

    contentItem: Items.BasicText {
        color: Style.textSecondaryColor
        style: Text.Raised
        styleColor: Style.textSecondaryColor
        font {
            pixelSize: s(Style.popupPixelSize)
            weight: Font.DemiBold
        }

        text: top.text
    }

    enter: Transition {
        ParallelAnimation {
            NumberAnimation { property: "opacity"; to: 1.0; duration: Style.animationDuration; easing.type: Style.animationEasing }
            NumberAnimation { property: "topMargin"; to: topMarginFinish; duration: Style.animationDuration; easing.type: Style.animationEasing }
        }
    }

    exit: Transition {
        ParallelAnimation {
            NumberAnimation { property: "opacity"; to: 0.0; duration: Style.animationDuration; easing.type: Style.animationEasing }
            NumberAnimation { property: "topMargin"; to: topMarginStart; duration: Style.animationDuration; easing.type: Style.animationEasing }
        }
    }

    Overlay.modal: Rectangle {
        color: Style.backgroundShadowColor

        Behavior on opacity {
            NumberAnimation { property: "opacity"; duration: Style.animationDuration; easing.type: Style.animationEasing }
        }
    }

    background: Rectangle {
        color: top.backgroundColor
        radius: top.height/2
    }
}
