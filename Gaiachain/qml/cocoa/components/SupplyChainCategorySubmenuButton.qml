import QtQuick 2.11
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0

import "../../items" as Items

Items.GButton {
    id: top

    padding: s(GStyle.middleMargin)
    spacing: s(GStyle.bigMargin)

    icon {
        height: s(GStyle.buttonImageTinyHeight)
        width: s(GStyle.buttonImageTinyHeight)
        source: GStyle.rightBlackArrowImgUrl
    }

    font {
        pixelSize: s(GStyle.submenuButtonPixelSize)
        letterSpacing: GStyle.none
        capitalization: Font.MixedCase
    }

    palette {
        button: GStyle.blank
        buttonText: GStyle.textPrimaryColor
    }

    disabledColorTint: GStyle.blank

    // this is to avoid reimplementing of contentItem
    Component.onCompleted: contentItem.alignment = Qt.AlignLeft
}
