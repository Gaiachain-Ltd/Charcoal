import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import "../items" as Items
import "../popups" as Popups

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

Popups.GenericPopup
{
    id: top

    property string text
    property var buttons: []
    property int additionalTextHorizontalMargins: 0

    padding: s(Style.bigMargin)
    topPadding: 2 * s(Style.bigMargin)

    contentItem: ColumnLayout {
        spacing: 2 * s(Style.bigMargin)

        Items.BasicText {
            Layout.fillWidth: true
            Layout.leftMargin: additionalTextHorizontalMargins
            Layout.rightMargin: additionalTextHorizontalMargins

            font {
                pixelSize: s(Style.popupPixelSize)
                weight: Font.DemiBold
            }
            horizontalAlignment: Qt.AlignHCenter

            text: top.text
        }
        RowLayout {
            spacing: s(Style.bigMargin)

            Repeater {
                model: buttons
                Items.GenericButton {
                    Layout.fillWidth: true
                    Layout.preferredWidth: 0    // this is to keep buttons size equal for all texts

                    palette {
                        buttonText: Style.textSecondaryColor
                        button: modelData.action === Enums.PopupAction.Accept ? Style.buttonPopupAcceptSecondaryColor : Style.buttonPopupRejectColor
                    }

                    text: modelData.text

                    onClicked: pageManager.sendAction(modelData.Action)
                }
            }
        }
    }
}
