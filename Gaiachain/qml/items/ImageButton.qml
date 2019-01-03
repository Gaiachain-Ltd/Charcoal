import QtQuick 2.12
import QtQuick.Controls 2.4

import com.gaiachain.style 1.0

Button {
    id: top

    property alias source: image.source
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

    property alias backgroundColor: backRect.color
    property alias backgroundRadius: backRect.radius

    property real inset: 0

    width: s(Style.buttonHeight)
    height: width

    leftInset: inset
    rightInset: inset
    topInset: inset
    bottomInset: inset

    contentItem: SvgImage {
        id: image
    }

    background: Rectangle {
        id: backRect
        color: "transparent"
        radius: s(20)
    }
}
