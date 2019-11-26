import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import Qt.labs.calendar 1.0

import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.enums 1.0

import "../items" as Items
import "../components" as Components

Items.GenericHeader {
    id: top

    property date currentDate: new Date
    property int currentYear: currentDate.getFullYear()
    property int currentMonth: currentDate.getMonth()

    property string dateText: currentDate.toLocaleDateString(Qt.locale(), qsTr("MM/dd/yyyy"))

    function previousMonth() {
        if (currentMonth === Calendar.January) {
            currentMonth = Calendar.December
            --currentYear
        } else {
            --currentMonth
        }
    }

    function nextMonth() {
        if (currentMonth === Calendar.December) {
            currentMonth = Calendar.January
            ++currentYear
        } else {
            ++currentMonth
        }
    }

    widget: Items.GenericInput {
        Layout.fillWidth: true

        text: top.dateText

        readOnly: true

        iconEdge: Enums.Edge.RightEdge
        iconSource: Style.calendarButtonImgUrl

        MouseArea {
            anchors.fill: parent
            onClicked: popup.visible = !popup.visible
        }
    }

    Popup {
        id: popup

        anchors.centerIn: Overlay.overlay

        width: top.width
        implicitHeight: width + s(Style.hugeMargin)

        focus: true
        modal: true

        Overlay.modal: Rectangle {
            color: Style.backgroundShadowColor
        }

        contentItem: ColumnLayout {
            Layout.fillWidth: false
            Layout.fillHeight: false

            Layout.bottomMargin: Style.none

            spacing: s(Style.hugeMargin)

            Components.CalendarNavigator {
                Layout.fillWidth: true

                returnButton: false

                currentMonth: calendarMonthItem.currentMonth
                currentYear: calendarMonthItem.currentYear

                onPrevious: previousMonth()
                onNext: nextMonth()
            }

            Components.CalendarMonthItem {
                id: calendarMonthItem

                currentDate: top.currentDate

                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: Style.none

                currentMonth: top.currentMonth
                currentYear: top.currentYear

                onDayClicked: {
                    currentDate = dayDate
                    top.dateText = dayDate.toLocaleDateString(Qt.locale(), qsTr("MM/dd/yyyy"))
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
            color: Style.backgroundColor
            radius: s(Style.tinyMargin)
        }
    }
}
