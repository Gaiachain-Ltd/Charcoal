import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11
import QtMultimedia 5.12
import QZXing 2.3

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.types 1.0
import com.gaiachain.static 1.0

import "../../common" as Common
import "../../components" as Components
import "../../headers" as Headers
import "../../pages" as Pages
import "../../popups" as Popups
import "../../items" as Items
import "../items" as CharcoalItems

Popups.GPopup
{
    id: popup

    readonly property int bigMargin: 3 * s(GStyle.bigMargin)

    property string text
    property string rejectButtonText
    property string acceptButtonText

    signal rejected()
    signal accepted()

    padding: 0
    width: parent.width - leftMargin - rightMargin
    height: mainLayout.preferredHeight

    ColumnLayout {
        id: mainLayout
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            margins: s(GStyle.bigMargin)
        }
        spacing: scannedBagsPopup.bigMargin

        Items.GText {
            Layout.fillWidth: true
            Layout.leftMargin: scannedBagsPopup.bigMargin
            Layout.rightMargin: scannedBagsPopup.bigMargin

            font {
                pixelSize: s(GStyle.bigPixelSize)
                weight: Font.DemiBold
            }

            horizontalAlignment: Qt.AlignHCenter
            wrapMode: Text.Wrap

            text: popup.text
        }

        ColumnLayout {
            spacing: s(GStyle.bigMargin)

            Items.GButton {
                Layout.fillWidth: true
                implicitHeight: 74

                palette {
                    buttonText: GStyle.textSecondaryColor
                    button: GStyle.buttonPopupRejectColor
                }

                text: rejectButtonText

                onClicked: rejected()
            }

            Items.GButton {
                Layout.fillWidth: true
                implicitHeight: 74

                palette {
                    buttonText: GStyle.textSecondaryColor
                    button: GStyle.buttonPopupAcceptSecondaryColor
                }

                text: acceptButtonText

                onClicked: accepted()
            }

            Item {
                // spacer
                width: 1
                implicitHeight: s(GStyle.bigMargin)
            }
        }
    }
}
