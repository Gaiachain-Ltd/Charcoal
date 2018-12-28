import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

import "../items" as Items

BasePage {
    id: top

    function enterCalendarPage() {
        pageManager.enterPage(Enums.Page.Calendar)
    }

    function enterMapPage() {
        console.log("TO_DO LATER: Implement map page!")
    }

    RowLayout {
        id: buttonLayout
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }

        property real buttonHeight: s(150)
        height: buttonHeight

        spacing: s(20)

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: buttonLayout.buttonHeight
        }

        Items.TextImageButton {
            Layout.preferredWidth: buttonLayout.buttonHeight

            buttonHeight: width

            source: Style.calendarImgUrl
            text: Strings.calendar
            textFont.pixelSize: s(30)

            onButtonClicked: top.enterCalendarPage()
        }

        Items.TextImageButton {
            Layout.preferredWidth: buttonLayout.buttonHeight

            buttonHeight: width

            source: Style.mapImgUrl
            text: Strings.map
            textFont.pixelSize: s(30)

            onButtonClicked: top.enterMapPage()
        }

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: buttonLayout.buttonHeight
        }
    }
}
