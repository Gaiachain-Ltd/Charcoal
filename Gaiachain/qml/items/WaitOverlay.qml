import QtQuick 2.11
import QtQuick.Controls 2.5

import com.gaiachain.style 1.0

import "../popups" as Popups

Popups.GenericPopup
{
    id: top
    height: s(Style.buttonHeight)
    width: s(Style.buttonHeight)

    background: Item {}
    contentItem: SvgImage {
        id: spinner

        source: Style.spinnerImgUrl

        RotationAnimator {
            target: spinner
            from: 0
            to: 360
            duration: 1000
            running: top.visible
            loops: Animation.Infinite
        }
    }
}
