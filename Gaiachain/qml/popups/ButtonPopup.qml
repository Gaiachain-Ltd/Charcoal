import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import "../items" as Items
import "../popups" as Popups

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

// NOTE: Use that popup if you want to define button logic
Popups.GPopup
{
    id: top

    property alias text: popupText.text
    property alias textFormat: popupText.textFormat
    property var buttons: []

    property color buttonPrimaryColor: GStyle.buttonPopupAcceptSecondaryColor
    property int additionalTextHorizontalMargins: 0

    padding: s(GStyle.bigMargin)
    topPadding: 2 * s(GStyle.bigMargin)

    contentItem: ColumnLayout {
        spacing: 2 * s(GStyle.bigMargin)

        Items.GText {
            id: popupText
            Layout.fillWidth: true
            Layout.leftMargin: additionalTextHorizontalMargins
            Layout.rightMargin: additionalTextHorizontalMargins

            font {
                pixelSize: s(GStyle.popupPixelSize)
                weight: Font.DemiBold
            }
            horizontalAlignment: Qt.AlignHCenter
            wrapMode: Text.Wrap
        }
        RowLayout {
            id: buttonsRow

            // required to keep proper size of the popup
            property int buttonHeight: 0
            Layout.preferredHeight: buttonHeight

            spacing: s(GStyle.bigMargin)

            Repeater {
                id: buttonsRepeater
                model: buttons

                function isAccept(action) {
                    return (action === Enums.PopupAction.Accept) ||
                        (action === Enums.PopupAction.Yes)
                }

                Items.GButton {
                    Layout.fillWidth: true
                    Layout.preferredWidth: 0    // this is to keep buttons size equal for all texts

                    palette {
                        buttonText: GStyle.textSecondaryColor
                        button: buttonsRepeater.isAccept(modelData.action) ? buttonPrimaryColor : GStyle.buttonPopupRejectColor
                    }

                    text: modelData.text

                    onClicked: pageManager.sendAction(modelData.action)

                    Component.onCompleted: {
                        buttonsRow.buttonHeight = implicitHeight
                    }
                }
            }
        }
    }
}
