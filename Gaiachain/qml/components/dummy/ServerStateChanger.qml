import QtQuick 2.12
import QtQuick.Controls 2.12

import "../../items" as Items

// this is for testing only !!!
Item
{
    anchors.fill: parent
    visible: typeof (fakeServer) !== "undefined"

    Items.GText {
        id: indicator
        anchors.fill: parent

        visible: typeof (fakeServer) !== "undefined" ? !fakeServer.up : false
        text: "DOWN"

        font.pixelSize: 1024
        fontSizeMode: Text.VerticalFit
        style: Text.Outline
        styleColor: "white"
        color: "red"

    }

    MouseArea {
        anchors.fill: parent

        enabled: typeof (fakeServer) !== "undefined"
        onDoubleClicked: fakeServer.upsideDown()
    }
}
