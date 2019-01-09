import QtQuick 2.12
import QtQuick.Controls 2.4

import com.gaiachain.style 1.0

Button {
    id: top

    property alias source: image.source
    property alias imageSize: image.height
    /* (just not to google it each time)
        Image.Stretch - the image is scaled to fit
        Image.PreserveAspectFit - the image is scaled uniformly to fit without cropping
        Image.PreserveAspectCrop - the image is scaled uniformly to fill, cropping if necessary
        Image.Tile - the image is duplicated horizontally and vertically
        Image.TileVertically - the image is stretched horizontally and tiled vertically
        Image.TileHorizontally - the image is stretched vertically and tiled horizontally
        Image.Pad - the image is not transformed
    */
    property alias fillMode: image.fillMode
    property alias showIcon: image.visible

    property alias backgroundColor: backRect.color
    property alias backgroundRadius: backRect.radius

    property alias textColor: buttonTextItem.color
    property alias textFont: buttonTextItem.font
    readonly property int textWidth: buttonTextItem.paintedWidth

    property real inset: 0

    implicitWidth: s(Style.buttonHeight)
    implicitHeight: s(Style.buttonHeight)

    leftInset: inset
    rightInset: inset
    topInset: inset
    bottomInset: inset

    contentItem: Item {
        id: innerItem
        Row {
            id: row
            anchors.centerIn: parent
            spacing: top.text !== "" && top.source !== "" ? s(Style.smallMargin) : 0
            BasicText {
                id: buttonTextItem
                text: top.text
                visible: text.length > 0
                anchors.verticalCenter: parent.verticalCenter
            }

            SvgImage {
                id: image
                height: innerItem.height
                width: height
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

    background: Rectangle {
        id: backRect
        color: "transparent"
        radius: s(Style.buttonRadius)
    }
}
