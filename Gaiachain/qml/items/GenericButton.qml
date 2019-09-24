import QtQuick 2.11
import QtQuick.Controls 2.5

import com.gaiachain.style 1.0

Button
{
    id: top

    property color borderColor: background.color

    font {
        pixelSize: s(Style.buttonPixelSize)
        family: Style.primaryFontFamily
        capitalization: Font.AllUppercase
        letterSpacing: s(Style.buttonLetterSpacing)
        weight: Font.DemiBold
    }

    background: Rectangle {
        id: background
        radius: s(Style.smallMargin)
        color: top.enabled ? palette.button : Qt.tint(palette.button, Style.disabledColorTint)
        border {
            width: sr(1)
            color: top.borderColor
        }
    }

    padding: s(Style.bigMargin)
}
