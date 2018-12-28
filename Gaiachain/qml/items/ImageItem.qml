import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0

Item {
    id: top

    implicitHeight: mainLayout.implicitHeight
    implicitWidth: mainLayout.implicitWidth

    property alias imageUrl: image.source

    // Use it to set image sizes.
    property real buttonWidth: -1.0
    property real buttonHeight: -1.0

    property alias text: text.text
    property alias textColor: text.color
    property alias textFont: text.font
    property alias spacing: mainLayout.spacing

    RowLayout {
        id: mainLayout

        anchors.centerIn: parent
        width: parent.width
        height: parent.height

        SvgImage {
            id: image

            Layout.fillWidth: top.buttonWidth < 0.0 ? true : false
            Layout.fillHeight: top.buttonHeight < 0.0 ? true : false

            Layout.preferredWidth: top.buttonWidth > 0.0 ? top.buttonWidth : implicitWidth
            Layout.preferredHeight: top.buttonHeight > 0.0 ? top.buttonHeight : implicitHeight

            fillMode: Image.PreserveAspectFit
        }

        Text {
            id: text

            Layout.fillHeight: true

            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter

            color: Style.textPrimaryColor
        }
    }

}
