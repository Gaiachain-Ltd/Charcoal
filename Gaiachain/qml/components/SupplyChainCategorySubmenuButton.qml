import QtQuick 2.11
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0

import "../items" as Items

Items.GenericButton {
    id: top

    padding: s(Style.middleMargin)
    spacing: s(Style.bigMargin)

    icon {
        height: s(Style.buttonImageTinyHeight)
        width: s(Style.buttonImageTinyHeight)
        source: Style.rightBlackArrowImgUrl
    }

    font {
        pixelSize: s(Style.submenuButtonPixelSize)
        letterSpacing: Style.none
        capitalization: Font.MixedCase
    }

    palette {
        button: Style.blank
        buttonText: Style.textPrimaryColor
    }

    disabledColorTint: Style.blank
}
