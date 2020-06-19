import QtQuick 2.12
import QtQuick.Controls 2.14

import com.gaiachain.style 1.0

TabButton {
    id: top

    property color selectedColor: GStyle.primaryColor
    property color deselectedColor: GStyle.backgroundSecondaryColor

    property color selectedFontColor: GStyle.primaryTextColor
    property color deselectedFontColor: GStyle.primaryTextColor

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

        implicitWidth: 100
        implicitHeight: 40

        color: top.deselectedColor
        radius: top.radius

        border.width: top.borderWidth
        border.color: top.borderColor
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
