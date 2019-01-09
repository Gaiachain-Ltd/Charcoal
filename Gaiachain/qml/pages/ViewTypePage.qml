import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

import "../items" as Items

BasePage {
    id: top

    function enterCalendarPage() {
        pageManager.enter(Enums.Page.Calendar)
    }

    function enterEventsListPage() {
        pageManager.enter(Enums.Page.EventsList)
    }

    function enterMapPage() {
        console.log("TO_DO LATER: Implement map page!")
    }

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            id: buttonLayout
            Layout.fillWidth: true
            property real margin: s(Style.bigMargin) * 1.5
            property real buttonHeight: s(150)
            Layout.preferredHeight: buttonHeight
            Layout.topMargin: margin
            Layout.leftMargin: margin

            spacing: margin

            Items.TextImageButton {
                Layout.preferredWidth: buttonLayout.buttonHeight

                buttonHeight: width
                buttonPadding: s(Style.normalMargin) * 1.1
                buttonBackgroundColor: Style.buttonBackColor

                shadowHorizontalOffset: 0
                shadowVericalOffset: s(15)

                source: Style.calendarImgUrl
                text: Strings.calendar
                textFont.pixelSize: s(30)

                spacing: s(Style.normalMargin)

                onButtonClicked: top.enterCalendarPage()
            }

            Items.TextImageButton {
                Layout.preferredWidth: buttonLayout.buttonHeight

                buttonHeight: width
                buttonPadding: s(Style.normalMargin) * 1.1
                buttonBackgroundColor: Style.buttonBackColor

                shadowHorizontalOffset: 0
                shadowVericalOffset: s(15)

                source: Style.listImgUrl
                text: Strings.list
                textFont.pixelSize: s(30)

                spacing: s(Style.normalMargin)

                onButtonClicked: top.enterEventsListPage()
            }
            Items.LayoutSpacer { spacerHeight: buttonLayout.buttonHeight }
        }
        Items.LayoutSpacer {}
    }
}
