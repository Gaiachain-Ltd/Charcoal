import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import Qt.labs.calendar 1.0

import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.enums 1.0

import "../items" as Items
import "../headers" as Headers
import "../components" as Components

Headers.GHeader {
    id: top

    property var selectedDate: optional ? undefined : currentDate

    property date currentDate: new Date
    property int currentYear: currentDate.getFullYear()
    property int currentMonth: currentDate.getMonth()

    property alias placeholderText: input.placeholderText

    property bool optional: false
    readonly property bool isEmpty: selectedDate === undefined

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

    function togglePopup() {
        popup.visible = !popup.visible
    }

    function clear() {
        selectedDate = undefined
    }

    widget: Items.GInput {
        id: input

        Layout.fillWidth: true
        Layout.leftMargin: top.margins
        Layout.rightMargin: top.margins

        text: isEmpty ? Strings.empty : top.selectedDate.toLocaleDateString(Qt.locale(), Strings.dateFormat)

        readOnly: true

        iconEdge: Enums.Edge.RightEdge
        iconSource: optional && !isEmpty ? GStyle.clearImgUrl : GStyle.calendarButtonImgUrl

        iconItem.z: ma.z + 1 // this is to handle icon click firstly

        onIconClicked: {
            if (optional && !isEmpty) {
                top.clear()
            } else {
                top.togglePopup()
            }
        }

        MouseArea {
            id: ma
            anchors.fill: parent
            onClicked: {
                forceActiveFocus()
                top.togglePopup()
            }
        }
    }

    Popup {
        id: popup

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

                onPrevious: previousMonth()
                onNext: nextMonth()
            }

            Components.CalendarMonthItem {
                id: calendarMonthItem

                currentDate: top.currentDate

                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: GStyle.none

                currentMonth: top.currentMonth
                currentYear: top.currentYear

                onDayClicked: {
                    top.currentDate = dayDate
                    top.selectedDate = dayDate
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
}
