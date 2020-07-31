import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../../items" as Items
import "../../common" as Common
import "../../headers" as Headers
import "../headers" as CharcoalHeaders
import "../../pages" as Pages

Pages.GPage {
    id: root

    property alias proceedButtonEnabled: proceedButton.enabled
    property var bagsMatch: undefined

    ColumnLayout {
        anchors.fill: parent

        Items.GText {
            Layout.fillWidth: true

            width: parent.width
            wrapMode: Text.WordWrap
            elide: Text.ElideNone
            horizontalAlignment: Text.AlignLeft
            text: Strings.missingBags
        }

        ListView {
            id: missingView

            Layout.fillWidth: true
            Layout.fillHeight: true

            model: bagsMatch.missingBags

            delegate: Item {
                width: missingView.width
                height: 80

                Items.GText {
                    anchors.fill: parent

                    text: modelData
                }
            }
        }

        Items.GText {
            Layout.fillWidth: true

            width: parent.width
            wrapMode: Text.WordWrap
            elide: Text.ElideNone
            horizontalAlignment: Text.AlignLeft
            text: Strings.extraBags
        }

        ListView {
            id: extraView

            Layout.fillWidth: true
            Layout.fillHeight: true

            model: bagsMatch.extraBags

            delegate: Item {
                width: extraView.width
                height: 80

                Items.GText {
                    anchors.fill: parent

                    text: modelData
                }
            }
        }

        Items.GButton {
            id: proceedButton

            Layout.leftMargin: s(GStyle.hugeMargin)
            Layout.rightMargin: Layout.leftMargin
            Layout.bottomMargin: Layout.leftMargin
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom

            padding: s(GStyle.bigMargin)

            enabled: true

            text: Strings.proceed

            onClicked: root.proceed()
        }
    }
}
