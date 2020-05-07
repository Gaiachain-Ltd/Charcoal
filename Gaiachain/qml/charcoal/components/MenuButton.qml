import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import com.gaiachain.style 1.0

import "../../items" as Items

Rectangle {
    property alias text: textItem.text
    property alias icon: image.source
    property alias fontColor: textItem.color

    signal clicked()

    id: top

    implicitHeight: s(GStyle.menuButtonHeight)

    Image {
        id: image

        anchors.fill: parent
        horizontalAlignment: Qt.AlignRight
        fillMode: Image.PreserveAspectFit
    }

    Items.GText {
        id: textItem
        anchors.fill: parent

        color: GStyle.textSecondaryColor
        wrapMode: Text.WordWrap
        textFormat: Text.PlainText
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
        font.letterSpacing: -0.7
        font.capitalization: Font.AllUppercase
        font.pixelSize: s(GStyle.bigPixelSize)
        font.bold: true
    }

    MouseArea {
        anchors.fill: parent
        onClicked: top.clicked()
    }
}
