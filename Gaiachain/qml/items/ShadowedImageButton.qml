import QtQuick 2.11
import QtGraphicalEffects 1.0

ImageButton {
    id: top

    property alias shadowColor: shadow.color
    property alias shadowHorizontalOffset: shadow.horizontalOffset
    property alias shadowVerticalOffset: shadow.verticalOffset

    DropShadow {
        id: shadow
        anchors.fill: background

        source: background
        horizontalOffset: -s(10)
        verticalOffset: s(10)
        radius: 8.0
        samples: 17
        color: "#80000000"
    }
}
