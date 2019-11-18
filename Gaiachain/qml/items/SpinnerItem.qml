import QtQuick 2.11

import com.gaiachain.style 1.0

import "../items" as Items

Items.SvgImage {
    id: top

    property bool running: true

    height: s(Style.buttonHeight)
    width: s(Style.buttonHeight)

    source: Style.spinnerImgUrl

    RotationAnimator {
        target: top
        from: 0
        to: 360
        duration: 1000
        running: top.running
        loops: Animation.Infinite
    }
}
