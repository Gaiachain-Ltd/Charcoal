import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import com.gaiachain.style 1.0

import "../items" as Items

Items.GenericButton
{
    id: top

    implicitHeight: s(Style.menuButtonHeight)
    spacing: s(Style.hugeMargin)

    topPadding: 0
    bottomPadding: 0

    borderColor: Style.buttonBorderColor
    borderRadius: s(Style.buttonSmallRadius)

    palette {
        button: Style.menuButtonColor
        buttonText: Style.textPrimaryColor
    }

    icon.height: s(Style.menuButtonImageHeight)
    icon.width: s(Style.menuButtonImageHeight)

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

        Items.BasicText {
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
