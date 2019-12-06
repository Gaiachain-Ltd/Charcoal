import QtQuick 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items

Items.InputHeader {
    id: top

    signal clicked()

    readOnly: true

    MouseArea {
        anchors.fill: parent
        onClicked: top.clicked()
    }
}
