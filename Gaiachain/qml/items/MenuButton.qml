import QtQuick 2.11
import QtQuick.Controls 2.5

import com.gaiachain.style 1.0

GenericButton
{
    id: top

    borderColor: Style.buttonBorderColor

    palette {
        button: Style.buttonMenuColor
        buttonText: Style.textPrimaryColor
    }

    icon.height: s(Style.buttonImageBigHeight)
    icon.width: s(Style.buttonImageBigHeight)

    font.capitalization: Font.MixedCase
    font.letterSpacing: 0

    spacing: s(Style.hugeMargin)

    contentItem: Row {
        spacing: top.spacing

        SvgImage {
            anchors.verticalCenter: parent.verticalCenter

            width: top.icon.width
            height: top.icon.height
            source: top.icon.source
        }

        BasicText {
            anchors.verticalCenter: parent.verticalCenter

            font: top.font
            text: top.text
        }
    }
}
