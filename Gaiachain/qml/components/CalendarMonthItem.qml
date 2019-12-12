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

    property alias delegate: grid.delegate
    property alias gridItem: grid

    property date currentDate

    property int currentMonth: currentDate.getMonth()
    property int currentYear: currentDate.getFullYear()

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
                text: Helper.formatShortName(model.shortName)
            }
        }

        MonthGrid {
            id: grid
            Layout.alignment: Qt.AlignHCenter

            property int minHeight: 0
            property int minWidth: 0
            property real scaleRatio: 1

            readonly property int monthLastDay: Helper.daysInMonth(grid.year, grid.month)
            property int lastDayWeekNumber: -1

            implicitWidth: minWidth
            implicitHeight: minHeight

            Layout.minimumHeight: minHeight
            Layout.minimumWidth: minWidth
            Layout.preferredWidth: height * scaleRatio
            Layout.fillHeight: true

            month: currentMonth
            year: currentYear

            spacing: Style.none

            onClicked: top.dayClicked(date)

            Component.onCompleted: {
                grid.minHeight = (s(Style.calendarDayMinHeight) + spacing) * grid.contentItem.rows
                grid.minWidth = (s(Style.calendarDayMinHeight) + spacing) * grid.contentItem.columns
                grid.scaleRatio = grid.contentItem.columns / grid.contentItem.rows
            }
        }
    }
}

