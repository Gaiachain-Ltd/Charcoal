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
    property alias minimumDate: calendarMonthItem.minimumDate

    onMinimumDateChanged: {
        if (minimumDate > selectedDate) {
            selectedDate = minimumDate
        }
    }

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
            Layout.bottomMargin: GStyle.bigMargin

            currentMonth: popup.currentDate.getMonth()
            currentYear: popup.currentDate.getFullYear()

            fontCapitalization: Font.AllUppercase
            dayRowFontColor: GStyle.textCalendarDayRowColor

            delegate: Rectangle {
                readonly property bool isCurrentMonth: (model.month === calendarMonthItem.gridItem.month)
                readonly property bool isBlockedDay: (isCurrentMonth && (model.day < calendarMonthItem.minimumDate.getDate()))
                readonly property bool isCurrentDay: (isCurrentMonth && model.day === selectedDate.getDate())

                border {
                    width: isCurrentDay? sr(3) : sr(1)
                    color: isCurrentDay? GStyle.calendarCurrentGridColor
                                       : GStyle.calendarGridColor
                }

                color: (isCurrentMonth && isBlockedDay)? GStyle.calendarBlockedColor
                                                       : GStyle.backgroundColor

                Text {
                    id: dayNumber
                    anchors.centerIn: parent

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    text: model.day
                    font.pixelSize: GStyle.calendarNumberPixelSize

                    color: {
                        if (isCurrentMonth) {
                            if (isBlockedDay) {
                                return GStyle.calendarBlockedFontColor
                            } else {
                                return GStyle.textPrimaryColor
                            }
                        }

                        return GStyle.calendarNotCurrentColor
                    }
                }
            }

            onDayClicked: {
                if (dayDate >= minimumDate) {
                    popup.currentDate = dayDate
                    popup.selectedDate = dayDate
                    popup.visible = false
                } else {
                    console.log("Wrong date! Selected:", dayDate,
                                "Minimum date:", minimumDate)
                    notificationPopup.open()
                }
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

        NotificationPopup {
            id: notificationPopup
            text: Strings.cantSelectThisDay
            backgroundColor: GStyle.notificationWarningColor
        }
    }
}
