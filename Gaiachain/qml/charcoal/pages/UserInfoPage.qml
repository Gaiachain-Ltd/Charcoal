import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.types 1.0

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

        contentHeight: menuColumn.implicitHeight + 2 * menuColumn.margins
        boundsBehavior: Flickable.StopAtBounds

        ColumnLayout {
            id: menuColumn

            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }

            spacing: 0

            Headers.InputHeader {
                id: nameHeader
                Layout.fillWidth: true
                readOnly: true
                borderWidth: 0
                summaryMode: true
                headerText: Strings.userInfoName
                inputText: userManager.userData.name
                helpButtonVisible: true
                helpText: Strings.userInfoNameHelp
            }

            Headers.InputHeader {
                id: jobHeader
                Layout.fillWidth: true
                readOnly: true
                borderWidth: 0
                summaryMode: true
                headerText: Strings.userInfoJob
                inputText: userManager.userData.job
                helpButtonVisible: true
                helpText: Strings.userInfoJobHelp
            }

            Headers.InputHeader {
                id: contactHeader
                Layout.fillWidth: true
                readOnly: true
                borderWidth: 0
                summaryMode: true
                headerText: Strings.userInfoContact
                inputText: userManager.userData.contact
                helpButtonVisible: true
                helpText: Strings.userInfoContactHelp
            }

            Headers.InputHeader {
                id: codeHeader
                Layout.fillWidth: true
                readOnly: true
                borderWidth: 0
                summaryMode: true
                headerText: Strings.userInfoCode
                inputText: userManager.userData.code
                helpButtonVisible: true
                helpText: Strings.userInfoCodeHelp
            }
        }
    }
}
