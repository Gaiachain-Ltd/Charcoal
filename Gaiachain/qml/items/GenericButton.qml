import QtQuick 2.11
import QtQuick.Controls 2.5

import com.gaiachain.style 1.0

Button
{
    id: top

    property color borderColor: background.color
    property int borderRadius: s(Style.buttonRadius)
    property int borderWidth: sr(Style.controlDefaultBorderWidth)

    property color disabledColorTint: Style.disabledColorTint

    font {
        pixelSize: s(Style.buttonPixelSize)
        family: Style.primaryFontFamily
        capitalization: Font.AllUppercase
        letterSpacing: s(Style.buttonLetterSpacing)
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

    padding: s(Style.bigMargin)
}
