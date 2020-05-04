import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0

import "../items" as Items
import "../popups" as Popups

Popups.TimedPopup {
    id: top

    property color backgroundColor: GStyle.primaryColor

    property string text

    readonly property int topMarginStart: (parent.height - height - s(GStyle.hugeMargin) * 6 - s(GStyle.transitionMoveYRange))
    readonly property int topMarginFinish: (parent.height - height - s(GStyle.hugeMargin) * 6)

    property alias iconSource: icon.source

    openedInterval: GStyle.notificationPopupOpenedDefaultInterval

    topMargin: topMarginStart
    padding: s(GStyle.hugeMargin)

    contentItem: ColumnLayout {
        spacing: (icon.hasImage ? s(GStyle.tinyMargin) : GStyle.none)

        Items.SvgImage {
            id: icon

            readonly property bool hasImage: (status === Image.Ready)

            visible: hasImage

            Layout.preferredHeight: s(GStyle.notificationPopupIconHeight)
            Layout.preferredWidth: s(GStyle.notificationPopupIconHeight)
            Layout.alignment: Qt.AlignHCenter
        }

        Items.GText {
            Layout.fillWidth: true

            color: GStyle.textSecondaryColor
            style: Text.Raised
            styleColor: GStyle.textSecondaryColor
            font {
                pixelSize: s(GStyle.popupPixelSize)
                weight: Font.DemiBold
            }

            wrapMode: Text.WordWrap
            text: top.text
        }

    }

    enter: Transition {
        ParallelAnimation {
            NumberAnimation { property: "opacity"; to: 1.0; duration: GStyle.animationDuration; easing.type: GStyle.animationEasing }
            NumberAnimation { property: "topMargin"; to: topMarginFinish; duration: GStyle.animationDuration; easing.type: GStyle.animationEasing }
        }
    }

    exit: Transition {
        ParallelAnimation {
            NumberAnimation { property: "opacity"; to: 0.0; duration: GStyle.animationDuration; easing.type: GStyle.animationEasing }
            NumberAnimation { property: "topMargin"; to: topMarginStart; duration: GStyle.animationDuration; easing.type: GStyle.animationEasing }
        }
    }

    Overlay.modal: Rectangle {
        color: GStyle.backgroundShadowColor

        Behavior on opacity {
            NumberAnimation { property: "opacity"; duration: GStyle.animationDuration; easing.type: GStyle.animationEasing }
        }
    }

    background: Rectangle {
        color: top.backgroundColor
        radius: s(GStyle.hugeMargin) * 1.5

        MouseArea {
            anchors.fill: parent
            onClicked: top.forceClose()
        }
    }
}
