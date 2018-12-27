import QtQuick 2.11
import QtQuick.Layouts 1.11

import "../items"

ImageButton {
    property alias bottomText: txt.text

    Text {
        id: txt

        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        font.pixelSize: s(30)
    }
}
