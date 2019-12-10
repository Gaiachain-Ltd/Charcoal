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

    widget: Items.GenericInput {
        id: input
        Layout.fillWidth: true

        text: isEmpty ? Strings.empty : top.selectedDate.toLocaleDateString(Qt.locale(), Strings.dateFormat)

        readOnly: true

        iconEdge: Enums.Edge.RightEdge
        iconSource: optional && !isEmpty ? Style.clearImgUrl : Style.calendarButtonImgUrl

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
            onClicked: top.togglePopup()
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
            color: Style.backgroundColor
            radius: s(Style.tinyMargin)
        }
    }
}
