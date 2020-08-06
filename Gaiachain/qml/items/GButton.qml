import QtQuick 2.11
import QtQuick.Controls 2.5

import com.gaiachain.style 1.0

Button
{
    id: top

    property color borderColor: background.color
    property int borderRadius: s(GStyle.buttonRadius)
    property int borderWidth: sr(GStyle.controlDefaultBorderWidth)

    property color disabledColorTint: GStyle.disabledColorTint

    property int fontPixelSize: s(GStyle.buttonPixelSize)

    font {
        pixelSize: fontPixelSize
        family: GStyle.primaryFontFamily
        capitalization: Font.AllUppercase
        letterSpacing: s(GStyle.buttonLetterSpacing)
        weight: Font.DemiBold
    }

    background: Rectangle {
        id: background
        radius: top.borderRadius
        color: top.enabled ? palette.button : Qt.tint(palette.button, top.disabledColorTint)
        border {
            width: top.borderWidth
            color: top.borderColor
        }
    }

    padding: s(GStyle.bigMargin)
}
