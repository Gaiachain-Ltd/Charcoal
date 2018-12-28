import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0

// TO_DO consider using Toolbar
Item {
    signal barClicked()

    Rectangle {
        id: background
        anchors.fill: parent
        color: Style.registerBarBackgroundColor
    }

    ImageItem {
        anchors {
            top: parent.top
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }

        buttonWidth: height

        imageUrl: Style.registerImgUrl

        text: Strings.registerRequest
        textColor: Style.textSecondaryColor
        textFont.pixelSize: s(30)

        spacing: s(40)
    }

    MouseArea {
        anchors.fill: parent
        onClicked: console.log("TO_DO: Implement registration request!")
    }
}
