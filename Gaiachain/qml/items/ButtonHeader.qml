import QtQuick 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items

Items.GenericHeader {
    id: top

    property alias iconSource: input.iconSource

    property string buttonText

    signal clicked()

    widget: Items.GenericInput {
        id: input

        Layout.fillWidth: true

        readOnly: true
        focus: false

        showIcon: true
        iconEdge: Enums.Edge.RightEdge

        text: top.buttonText

        MouseArea {
            anchors.fill: parent
            onClicked: top.clicked()
        }
    }
}
