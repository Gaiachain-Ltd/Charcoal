import QtQuick 2.12
import QtQuick.Controls 2.14

import com.gaiachain.style 1.0

TabButton {
    id: top

    contentItem: GText {
        text: top.text
        font: top.font
    }

    background: Rectangle {
        id: backgroundRect

        implicitWidth: 100
        implicitHeight: 40

        color: GStyle.backgroundSecondaryColor

        states: State {
            when: top.checked
            PropertyChanges { target: backgroundRect; color: GStyle.primaryColor }
        }

        transitions: Transition {
            ColorAnimation { duration: GStyle.animationDuration }
        }
    }
}
