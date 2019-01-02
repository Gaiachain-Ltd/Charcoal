import QtQuick 2.12
import QtQuick.Controls 2.4

Button {
    id: top

    property alias source: image.source
    property alias fillMode: image.fillMode

    property alias backgroundColor: backRect.color
    property alias backgroundRadius: backRect.radius

    property real inset: 0
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
