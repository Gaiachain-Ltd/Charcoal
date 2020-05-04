import QtQuick 2.11

import com.gaiachain.style 1.0

import "../items" as Items

Items.SvgImage {
    id: top

    property bool running: true

    height: s(GStyle.buttonHeight)
    width: s(GStyle.buttonHeight)

    source: GStyle.spinnerImgUrl

    RotationAnimator {
        target: top
        from: 0
        to: 360
        duration: 1000
        running: top.running
        loops: Animation.Infinite
    }
}
