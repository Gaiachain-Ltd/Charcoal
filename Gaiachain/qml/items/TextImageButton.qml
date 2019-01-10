import QtQuick 2.11
import QtQuick.Layouts 1.11

import "../items"

import com.gaiachain.style 1.0

Item {
    id: top

    implicitHeight: mainLayout.implicitHeight
    implicitWidth: mainLayout.implicitWidth

    property alias source: shadowedButton.source
    property alias buttonInset: shadowedButton.inset
    property alias buttonPadding: shadowedButton.padding
    property alias buttonBackgroundColor: shadowedButton.backgroundColor

    property alias shadowColor: shadowedButton.shadowColor
    property alias shadowHorizontalOffset: shadowedButton.shadowHorizontalOffset
    property alias shadowVericalOffset: shadowedButton.shadowVerticalOffset

    // Use it to set button sizes. If not set background and button will stretch.
    property real buttonWidth: -1.0
    property real buttonHeight: -1.0

    signal buttonClicked()

    property alias text: text.text
    property alias textFont: text.font
    property alias textHorizontalAlignment: text.horizontalAlignment

    property alias spacing: mainLayout.spacing

    ColumnLayout {
        id: mainLayout

        anchors.centerIn: parent
        width: parent.width
        height: parent.height

        ShadowedImageButton {
            id: shadowedButton

            Layout.fillWidth: top.buttonWidth < 0.0 ? true : false
            Layout.fillHeight: top.buttonHeight < 0.0 ? true : false

            Layout.preferredWidth: top.buttonWidth > 0.0 ? top.buttonWidth : implicitWidth
            Layout.preferredHeight: top.buttonHeight > 0.0 ? top.buttonHeight : implicitHeight

            onClicked: top.buttonClicked()
        }

        BasicText {
            id: text
            Layout.fillWidth: true
        }
    }
}
