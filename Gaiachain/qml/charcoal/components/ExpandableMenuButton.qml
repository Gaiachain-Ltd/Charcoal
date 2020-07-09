import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import com.gaiachain.style 1.0

import "../../items" as Items

MenuButton {
    id: top

    property bool expanded: false

    onClicked: expanded = !expanded

    Image {
        anchors {
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
            bottomMargin: 5
        }

        horizontalAlignment: Image.AlignHCenter
        verticalAlignment: Image.AlignVCenter

        source: GStyle.arrowImgUrl
        rotation: top.expanded? 0 : 180
    }
}
