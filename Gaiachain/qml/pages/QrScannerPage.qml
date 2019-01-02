import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

import "../items" as Items

BasePage {
    id: top

    ColumnLayout
    {
        anchors.fill: parent

        spacing: 0

        Rectangle
        {
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "red"
        }

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: s(Style.footerHeight)

            RowLayout
            {
                anchors {
                    fill: parent
                    leftMargin: s(30)
                    rightMargin: s(30)
                    topMargin: s(15)
                    bottomMargin: s(15)
                }
                spacing: s(30)

                Items.BasicText
                {
                    text: Strings.scanning
                    horizontalAlignment: Text.AlignLeft
                    font.pixelSize: s(Style.bigPixelSize)
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

                Rectangle
                {
                    color: "grey"
                    Layout.preferredWidth: s(Style.buttonHeight)
                    Layout.preferredHeight: s(Style.buttonHeight)
                }

                Rectangle
                {
                    color: "grey"
                    opacity: 0.5
                    Layout.preferredWidth: s(Style.buttonHeight)
                    Layout.preferredHeight: s(Style.buttonHeight)
                }

            }
        }
    }

}
