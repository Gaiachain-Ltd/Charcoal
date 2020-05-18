import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import Qt.labs.calendar 1.0

import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.enums 1.0

import "../components" as Components
import "../pages" as Pages

Popup {
    id: popup

    property date currentDate: new Date
    property date selectedDate: currentDate

    property bool showCloseButton: false
    property bool logoVisible: false

    property string title: Strings.selectDate

    anchors.centerIn: Overlay.overlay
    width: Overlay.overlay? Overlay.overlay.width : 100
    height: Overlay.overlay? Overlay.overlay.height : 100

    focus: true
    modal: true

    ColumnLayout {
        anchors.fill: parent

        spacing: 0

        Components.NavigationHeader {
            id: header
            Layout.fillWidth: true

            title: popup.title
            showCloseButton: popup.showCloseButton
            showBackButton: true
            enableBackButton: true
            logoVisible: popup.logoVisible

            // Reimplemented NavigationHeader.goBack()
            function goBack() {
                popup.close()
            }
        }

        Item {
            Layout.fillHeight: true

            id: spacer1
            width: 1
        }

        Components.CalendarNavigator {
            Layout.fillWidth: true
            Layout.bottomMargin: s(GStyle.middleMargin)

            returnButton: false
            sparseLayout: true
            fontCapitalization: Font.AllUppercase

            currentMonth: calendarMonthItem.currentMonth
            currentYear: calendarMonthItem.currentYear

            onPrevious: currentDate = Utility.previousMonth(currentDate)
            onNext: currentDate = Utility.nextMonth(currentDate)
        }

        Components.CalendarMonthItem {
            id: calendarMonthItem

            currentDate: popup.currentDate

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: GStyle.none

            currentMonth: popup.currentDate.getMonth()
            currentYear: popup.currentDate.getFullYear()

            fontCapitalization: Font.AllUppercase

            onDayClicked: {
                popup.currentDate = dayDate
                popup.selectedDate = dayDate
                popup.visible = false
            }

            delegate: Components.CalendarSupplyItem {
                Components.CalendarItemGridExtension {
                    id: gridExtension

                    modelDate: model.date
                    modelDay: model.day
                    modelMonth: model.month
                    modelWeekNumber: model.weekNumber

                    gridItem: calendarMonthItem.gridItem
                }

                visible: !gridExtension.additionalRow
                selectedDate: calendarMonthItem.currentDate
                currentMonth: calendarMonthItem.currentMonth
            }
        }

        Item {
            Layout.fillHeight: true

            id: spacer2
            width: 1
        }

        Components.Footer {
            id: footer
            Layout.fillWidth: true
        }
    }
}
