import QtQuick 2.12
import QtQuick.Controls 2.4

import com.gaiachain.style 1.0

GButton {
    id: top

    property alias source: image.source
    property alias fillMode: image.fillMode
    property alias status: image.status

    borderRadius: s(GStyle.buttonSmallRadius)
    padding: s(GStyle.buttonImageMargin)

    implicitWidth: s(GStyle.buttonImageHeight)
    implicitHeight: s(GStyle.buttonImageHeight)

    contentItem: SvgImage { id: image }
}
