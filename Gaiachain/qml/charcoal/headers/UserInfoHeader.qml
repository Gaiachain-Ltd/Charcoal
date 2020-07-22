import QtQuick 2.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../../headers" as Headers

Headers.InputHeader {
    id: top

    property alias clickEnabled: mouseArea.enabled

    headerText: Strings.malebiRepsId
    readOnly: true
    helpButtonVisible: true
    inputText: userManager.userData.code

    MouseArea {
        id: mouseArea

        anchors {
            fill: parent
            topMargin: widgetX
        }

        onClicked: pageManager.enter(Enums.Page.UserInfo)
        // Disabled because of client request:
        // https://projects.milosolutions.com/issues/87877
        enabled: false
    }
}
