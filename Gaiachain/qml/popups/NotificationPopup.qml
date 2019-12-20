import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0

import "../items" as Items
import "../popups" as Popups

Popups.TimedPopup {
    id: top

    property color backgroundColor: Style.primaryColor

    property string text

    readonly property int topMarginStart: (parent.height - height - s(Style.hugeMargin) * 6 - s(Style.transitionMoveYRange))
    readonly property int topMarginFinish: (parent.height - height - s(Style.hugeMargin) * 6)

    property alias iconSource: icon.source

    openedInterval: Style.notificationPopupOpenedDefaultInterval

    topMargin: topMarginStart
    padding: s(Style.hugeMargin)

    contentItem: ColumnLayout {
        spacing: (icon.hasImage ? s(Style.tinyMargin) : Style.none)

        Items.SvgImage {
            id: icon

            readonly property bool hasImage: (status === Image.Ready)

            visible: hasImage

            Layout.preferredHeight: s(Style.notificationPopupIconHeight)
            Layout.preferredWidth: s(Style.notificationPopupIconHeight)
            Layout.alignment: Qt.AlignHCenter
        }

        Items.BasicText {
            Layout.fillWidth: true

            color: Style.textSecondaryColor
            style: Text.Raised
            styleColor: Style.textSecondaryColor
            font {
                pixelSize: s(Style.popupPixelSize)
                weight: Font.DemiBold
            }

            wrapMode: Text.WordWrap
            text: top.text
        }

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
        radius: s(Style.hugeMargin) * 1.5

        MouseArea {
            anchors.fill: parent
            onClicked: top.forceClose()
        }
    }
}
