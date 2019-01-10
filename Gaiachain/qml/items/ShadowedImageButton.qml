import QtQuick 2.11
import QtGraphicalEffects 1.0

import com.gaiachain.style 1.0

Item {
    id: top

    property alias shadowColor: shadow.color
    property alias shadowHorizontalOffset: shadow.horizontalOffset
    property alias shadowVerticalOffset: shadow.verticalOffset

    property alias inset: image.inset
    property alias padding: image.padding
    property alias source: image.source
    property alias backgroundColor: image.backgroundColor

    signal clicked()

    ImageButton {
        id: image
        anchors.fill: parent

        backgroundRadius: s(Style.bigButtonRadius)

        onClicked: top.clicked()
    }

    DropShadow {
        id: shadow
        anchors.fill: image

        source: image
        horizontalOffset: -s(10)
        verticalOffset: s(10)
        samples: radius * 2 + 1
        radius: 16
        color: "#50000000"
    }
}
