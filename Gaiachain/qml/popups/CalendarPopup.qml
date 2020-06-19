import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import Qt.labs.calendar 1.0

import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.enums 1.0

import "../components" as Components

Popup {
    id: popup

    property date currentDate: new Date
    property date selectedDate: currentDate

    anchors.centerIn: Overlay.overlay

    width: top.width
    implicitHeight: width + s(GStyle.hugeMargin)

    focus: true
    modal: true

    Overlay.modal: Rectangle {
        color: GStyle.backgroundShadowColor
    }

    contentItem: ColumnLayout {
        Layout.fillWidth: false
        Layout.fillHeight: false

        Layout.bottomMargin: GStyle.none

        spacing: s(GStyle.hugeMargin)

        Components.CalendarNavigator {
            Layout.fillWidth: true

            returnButton: false

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
    }

    background: Rectangle {
        color: GStyle.backgroundColor
        radius: s(GStyle.tinyMargin)
    }
}
