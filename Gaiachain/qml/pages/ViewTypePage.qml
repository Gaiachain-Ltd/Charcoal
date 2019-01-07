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

    function enterEventsListPage() {
        pageManager.enterPage(Enums.Page.EventsList)
    }

    function enterMapPage() {
        console.log("TO_DO LATER: Implement map page!")
    }

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            id: buttonLayout
            Layout.fillWidth: true
            property real buttonHeight: s(150)
            Layout.preferredHeight: buttonHeight

            spacing: s(20)

            Items.LayoutSpacer { spacerHeight: buttonLayout.buttonHeight }
            Items.TextImageButton {
                Layout.preferredWidth: buttonLayout.buttonHeight

                buttonHeight: width
                buttonPadding: s(22)
                buttonBackgroundColor: Style.buttonBackColor

                shadowHorizontalOffset: s(5)
                shadowVericalOffset: s(15)

                source: Style.calendarImgUrl
                text: Strings.calendar
                textFont.pixelSize: s(30)

                onButtonClicked: top.enterCalendarPage()
            }

            Items.TextImageButton {
                Layout.preferredWidth: buttonLayout.buttonHeight

                buttonHeight: width
                buttonPadding: s(22)
                buttonBackgroundColor: Style.buttonBackColor

                shadowHorizontalOffset: s(5)
                shadowVericalOffset: s(15)

                source: Style.listImgUrl
                text: Strings.list
                textFont.pixelSize: s(30)

                onButtonClicked: top.enterEventsListPage()
            }
            Items.LayoutSpacer { spacerHeight: buttonLayout.buttonHeight }
        }
        Items.LayoutSpacer {}
    }
}
