import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0

TabButton {
    id: top

    property color selectedColor: GStyle.primaryColor
    property color deselectedColor: GStyle.backgroundSecondaryColor

    property color selectedFontColor: GStyle.primaryTextColor
    property color deselectedFontColor: GStyle.primaryTextColor

    readonly property alias textWidth: contentItemRect.contentWidth

    property color borderColor: "#ffffff"
    property int borderWidth: 0

    property int radius: 0

    contentItem: GText {
        id: contentItemRect
        text: top.text
        font: top.font
        color: top.deselectedFontColor
    }

    background: Rectangle {
        id: backgroundRect

        implicitWidth: textWidth + s(GStyle.hugeMargin)
        implicitHeight: 40

        color: top.deselectedColor
        radius: top.radius

        border.width: top.borderWidth
        border.color: top.borderColor

        // "Mask" bottom radius so that item looks rectangular on the bottom
        // TODO: create custom QQuickItem instead!
        RowLayout {
            spacing: 0
            height: top.radius
            anchors {
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }

            // Left "border"
            Rectangle {
                color: top.borderColor
                height: top.radius
                width: top.borderWidth
            }

            // Empty middle
            Rectangle {
                Layout.fillWidth: true
                color: backgroundRect.color
                height: top.radius
            }

            // Right "border"
            Rectangle {
                color: top.borderColor
                height: top.radius
                width: top.borderWidth
            }
        }
    }

    states: State {
        when: top.checked
        PropertyChanges { target: backgroundRect; color: top.selectedColor }
        PropertyChanges { target: contentItemRect; color: top.selectedFontColor }
    }

    transitions: Transition {
        ColorAnimation { duration: GStyle.animationDuration }
    }
}
