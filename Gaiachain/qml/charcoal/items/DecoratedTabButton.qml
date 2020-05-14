import QtQuick 2.12
import QtQuick.Controls 2.12

import "../../items" as Items

import com.gaiachain.style 1.0

// TODO: modularize GTabButton instead!
TabButton {
    id: tab
    property bool isCurrent: tabBar.currentItem === this
    text: Strings.allTab

    contentItem: Items.GText {
        text: tab.text
        font.capitalization: Font.AllUppercase
        color: tab.isCurrent? GStyle.backgroundColor
                            : GStyle.textTabColor
    }

    background: Rectangle {
        radius: s(5)
        border.color: GStyle.inputBorderColor
        border.width: sr(1)
        color: tab.isCurrent? GStyle.headerBackgroundColor
                            : GStyle.backgroundColor

//        states: State {
//            when: top.checked
//            PropertyChanges { target: backgroundRect; color: GStyle.primaryColor }
//        }

//        transitions: Transition {
//            ColorAnimation { duration: GStyle.animationDuration }
//        }
    }
}
