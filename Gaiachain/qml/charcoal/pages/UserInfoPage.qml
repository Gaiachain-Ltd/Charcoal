import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../../common" as Common
import "../../headers" as Headers
import "../headers" as CharcoalHeaders
import "../../pages" as Pages

Pages.GPage {
    id: top

    title: Strings.userInfoPageTitle
    logoVisible: false
    showCloseButton: false

    function closeEventHandler() {
        pageManager.back()
        return false
    }

    Flickable {
        anchors.fill: parent

        contentHeight: menuColumn.height
        boundsBehavior: Flickable.StopAtBounds

        Column {
            id: menuColumn

            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }

            spacing: 0

            Headers.InputHeader {
                id: nameHeader
                width: parent.width
                readOnly: true
                borderWidth: 0
                summaryMode: true
                headerText: Strings.userInfoName
                inputText: userManager.userData.name
                helpButtonVisible: false
            }

            Headers.InputHeader {
                id: jobHeader
                width: parent.width
                readOnly: true
                borderWidth: 0
                summaryMode: true
                headerText: Strings.userInfoJob
                inputText: userManager.userData.job
                helpButtonVisible: false
            }

            Headers.InputHeader {
                id: contactHeader
                width: parent.width
                readOnly: true
                borderWidth: 0
                summaryMode: true
                headerText: Strings.userInfoContact
                inputText: userManager.userData.contact
                helpButtonVisible: false
            }

            Headers.InputHeader {
                id: codeHeader
                width: parent.width
                readOnly: true
                borderWidth: 0
                summaryMode: true
                headerText: Strings.userInfoCode
                inputText: userManager.userData.code
                helpButtonVisible: false
            }
        }
    }
}
