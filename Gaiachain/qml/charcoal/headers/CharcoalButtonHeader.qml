import QtQuick 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../../items" as Items
import "../../headers" as Headers

Headers.GHeader {
    id: root

    property string text
    property string extraText
    property alias icon: iconComponent.source
    property alias iconVisible: iconComponent.visible

    property bool optional: false
    readonly property bool isEmpty: (text === Strings.empty)

    property bool forceBoldValue: false
    property int valueFontSize: s(GStyle.inputPixelSize)

    signal clicked()

    function clear() {
        text = Strings.empty
    }

    widget: Items.GText {
        id: textWidget

        Layout.fillWidth: true
        Layout.leftMargin: root.margins
        Layout.rightMargin: root.margins

        focus: false
        color: secondaryTextColor
        font.bold: root.highlighted || forceBoldValue
        font.pixelSize: valueFontSize

        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignLeft

        bottomInset: 0
        topInset: 0

        padding: root.margins

        textFormat: Text.StyledText
        text: root.text + (extraText.length > 0? "<br/>" + extraText : "")

        background: Rectangle {
            color: GStyle.blank
            radius: s(GStyle.tinyMargin)
            border.color: GStyle.inputBorderColor
            border.width: sr(1)
        }

        Image {
            id: iconComponent
            source: GStyle.checkGreenUrl
            visible: false
            width: s(40)
            height: width
            anchors {
                verticalCenter: parent.verticalCenter
                right: parent.right
                rightMargin: s(GStyle.middleMargin)
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: root.clicked()
        }
    }
}
