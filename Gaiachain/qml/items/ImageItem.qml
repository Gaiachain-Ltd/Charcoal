import QtQuick 2.11
import QtQuick.Controls 2.5

import com.gaiachain.style 1.0

Control {
    id: top

    property alias source: image.source
    property alias fillMode: image.fillMode

    property alias backgroundColor: background.color
    property int borderRadius: s(Style.buttonSmallRadius)

    implicitWidth: s(Style.buttonImageHeight)
    implicitHeight: s(Style.buttonImageHeight)

    padding: s(Style.buttonImageMargin)

    contentItem: SvgImage { id: image }

    background: Rectangle {
        id: background

        color: palette.window
        radius: top.borderRadius
    }
}
