import QtQuick 2.12
import QtQuick.Controls 2.4

Button {
    id: top

    property alias source: image.source
    property alias fillMode: image.fillMode

    property real inset: 0
    leftInset: inset
    rightInset: inset
    topInset: inset
    bottomInset: inset

    background: SvgImage {
        id: image
    }
}
