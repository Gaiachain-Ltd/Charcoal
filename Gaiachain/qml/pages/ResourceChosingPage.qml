import QtQuick 2.11

import com.gaiachain.enums 1.0

BasePage {

    Rectangle {
        anchors.centerIn: parent
        color: "blue"
        width: 50
        height: 50

        MouseArea {
            anchors.fill: parent
            onClicked: {
                pageManager.enterPage(Enums.Page.ViewType)
            }
        }
    }
}
