import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import com.gaiachain.style 1.0

import "../items" as Items

Items.GButton
{
    id: top

    implicitHeight: s(GStyle.menuButtonHeight)
    spacing: s(GStyle.hugeMargin)

    topPadding: 0
    bottomPadding: 0

    borderColor: GStyle.buttonBorderColor
    borderRadius: s(GStyle.buttonSmallRadius)

    palette {
        button: GStyle.menuButtonColor
        buttonText: GStyle.textPrimaryColor
    }

    icon.height: s(GStyle.menuButtonImageHeight)
    icon.width: s(GStyle.menuButtonImageHeight)

    font.capitalization: Font.MixedCase
    font.letterSpacing: 0

    contentItem: RowLayout {
        spacing: top.spacing

        Items.SvgImage {
            Layout.maximumHeight: availableHeight
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredWidth: top.icon.width
            Layout.preferredHeight: top.icon.height

            source: top.icon.source
        }

        Items.GText {
            Layout.maximumHeight: availableHeight
            Layout.alignment: Qt.AlignVCenter
            Layout.fillWidth: true

            font: top.font
            text: top.text

            wrapMode: Text.WordWrap
            horizontalAlignment: Qt.AlignLeft
        }
    }
}
