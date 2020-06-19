import QtQuick 2.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../../headers" as Headers

Headers.InputHeader {
    id: top

    headerText: Strings.malebiRepsId
    readOnly: true
    helpButtonVisible: true
    inputText: userManager.userData.code

    MouseArea {
        anchors.fill: parent
        onClicked: pageManager.enter(Enums.Page.UserInfo)
    }
}
