import QtQuick 2.11
import QtQuick.Controls 2.5

import com.gaiachain.style 1.0

Control {
    id: top

    property alias source: image.source
    property alias fillMode: image.fillMode

    property alias backgroundColor: background.color
    property int borderRadius: s(GStyle.buttonSmallRadius)

    implicitWidth: s(GStyle.buttonImageHeight)
    implicitHeight: s(GStyle.buttonImageHeight)

    padding: s(GStyle.buttonImageMargin)

    contentItem: SvgImage { id: image }

    background: Rectangle {
        id: background

        color: palette.window
        radius: top.borderRadius
    }
}
