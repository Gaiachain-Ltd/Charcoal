import QtQuick 2.11
import QtQuick.Layouts 1.11

import Qt.labs.calendar 1.0

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items

Item {
    id: top
    implicitWidth: topColumn.implicitWidth
    implicitHeight: topColumn.implicitHeight

    property int currentMonth
    property int currentYear

    signal dayClicked(date dayDate)

    ColumnLayout {
        id: topColumn
        anchors.fill: parent
        spacing: s(Style.tinySpacing)

        DayOfWeekRow {
            id: namesRow
            Layout.preferredWidth: grid.width
            Layout.alignment: Qt.AlignHCenter

            locale: grid.locale

            delegate: Items.BasicText {
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                font.pixelSize: s(Style.calendarNamePixelSize)
                text: model.shortName
            }
        }
        MonthGrid {
            id: grid
            Layout.alignment: Qt.AlignHCenter

            property int minHeight: 0
            property int minWidth: 0
            property real scaleRatio: 1

            implicitWidth: minWidth
            implicitHeight: minHeight

            Layout.minimumHeight: minHeight
            Layout.minimumWidth: minWidth
            Layout.preferredWidth: height * scaleRatio
            Layout.fillHeight: true

            month: currentMonth
            year: currentYear

            spacing: 0

            onClicked: top.dayClicked(date)

            Component.onCompleted: {
                grid.minHeight = (s(Style.calendarDayMinHeight) + spacing) * grid.contentItem.rows
                grid.minWidth = (s(Style.calendarDayMinHeight) + spacing) * grid.contentItem.columns
                grid.scaleRatio = grid.contentItem.columns / grid.contentItem.rows
            }

            readonly property int monthLastDay: Helpers.daysInMonth(grid.year, grid.month)
            property int lastDayWeekNumber: -1

            delegate: CalendarItemDelegate {
                currentMonth: grid.month

                function isAdditionalRow() {
                    if (model.month <= grid.month) {
                        if (model.day === grid.monthLastDay) {
                            grid.lastDayWeekNumber = model.weekNumber
                        }
                        return false
                    }

                    // a day from next month
                    return (model.weekNumber > grid.lastDayWeekNumber) ||
                            (model.date.getDay() === 0 && grid.locale.firstDayOfWeek === Locale.Sunday)
                }
                visible: !isAdditionalRow()
            }
        }
    }
}

