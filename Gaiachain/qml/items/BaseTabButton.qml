import QtQuick 2.14
import QtQuick.Controls 2.14

import com.gaiachain.style 1.0

TabButton {
    id: top

    contentItem: BasicText {
        text: top.text
        font: top.font
    }

    background: Rectangle {
        id: backgroundRect

        implicitWidth: 100
        implicitHeight: 40

        color: Style.backgroundSecondaryColor

        states: State {
            when: top.checked
            PropertyChanges { target: backgroundRect; color: Style.primaryColor }
        }

        transitions: Transition {
            ColorAnimation { duration: Style.animationDuration }
        }
    }
}
