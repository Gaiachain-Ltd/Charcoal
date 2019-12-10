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

    property color buttonPrimaryColor: Style.buttonPopupAcceptSecondaryColor
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
            wrapMode: Text.Wrap

            text: top.text
        }
        RowLayout {
            id: buttonsRow

            // required to keep proper size of the popup
            property int buttonHeight: 0
            Layout.preferredHeight: buttonHeight

            spacing: s(Style.bigMargin)

            Repeater {
                id: buttonsRepeater
                model: buttons

                function isAccept(action) {
                    return (action === Enums.PopupAction.Accept) ||
                        (action === Enums.PopupAction.Yes)
                }

                Items.GenericButton {
                    Layout.fillWidth: true
                    Layout.preferredWidth: 0    // this is to keep buttons size equal for all texts

                    palette {
                        buttonText: Style.textSecondaryColor
                        button: buttonsRepeater.isAccept(modelData.action) ? buttonPrimaryColor : Style.buttonPopupRejectColor
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
