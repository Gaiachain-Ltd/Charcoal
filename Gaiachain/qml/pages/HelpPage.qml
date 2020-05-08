import QtQuick 2.12
import QtQuick.Layouts 1.3

import "../items" as Items

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

Items.GPanel {
    id: top

    property alias text: helpText.text

    property int page: Enums.Page.Help
    property bool isPopup: false

    function initialize() {}

    function closeEventHandler()
    {
        pageManager.back();
        return false;
    }

    Rectangle {
        anchors.fill: parent
        color: GStyle.helpPageBackgroundColor
    }

    Image {
        source: GStyle.helpPageImageUrl
        anchors {
            right: parent.right
            bottom: parent.bottom
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: s(GStyle.bigMargin)
        spacing: s(GStyle.bigMargin)

        RowLayout {
            Items.GText {
                Layout.fillWidth: true
                text: Strings.helpTitle.arg(top.title)
                color: GStyle.textSecondaryColor
                horizontalAlignment: Text.AlignLeft
                font {
                    capitalization: Font.AllUppercase
                    pixelSize: s(GStyle.bigPixelSize)
                }
            }

            Image {
                source: GStyle.clearImgUrl

                MouseArea {
                    anchors.fill: parent
                    onClicked: pageManager.back()
                }
            }
        }

        Items.GText {
            id: helpText
            Layout.fillHeight: true
            Layout.fillWidth: true
            text: Strings.defaultHelp
            color: GStyle.textSecondaryColor
            wrapMode: Text.Wrap
            verticalAlignment: Text.AlignTop
            horizontalAlignment: Text.AlignLeft
            font {
                italic: true
                pixelSize: s(GStyle.titlePixelSize)
            }
        }
    }
}
