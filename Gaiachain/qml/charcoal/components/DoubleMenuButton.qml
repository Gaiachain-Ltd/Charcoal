import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import com.gaiachain.style 1.0

import "../../items" as Items

MenuButton {
    id: top

    property alias leftText: left.text
    property alias rightText: right.text

    property alias leftIcon: left.icon
    property alias rightIcon: right.icon

    property alias leftColor: left.color
    property alias rightColor: right.color

    signal leftClicked()
    signal rightClicked()

    property bool open: false
    onClicked: {
        if (open === false) {
            open = true;
        }

        // When `open` is true, we ignore the click
    }

    Row {
        id: row
        readonly property int half: top.width * .5

        anchors.fill: parent
        visible: top.open

        MenuButton {
            id: left
            height: top.height
            width: row.half
            fontColor: GStyle.textPrimaryColor
            onClicked: leftClicked()
        }

        MenuButton {
            id: right
            height: top.height
            width: row.half
            fontColor: GStyle.textPrimaryColor
            onClicked: rightClicked()
        }
    }
}
