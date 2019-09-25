import QtQuick 2.12
import QtQuick.Controls 2.4

import com.gaiachain.style 1.0

GenericButton {
    id: top

    property alias source: image.source
    property alias fillMode: image.fillMode

    borderRadius: s(Style.buttonSmallRadius)
    padding: s(Style.buttonImageMargin)

    implicitWidth: s(Style.buttonImageHeight)
    implicitHeight: s(Style.buttonImageHeight)

    contentItem: SvgImage { id: image }
}
