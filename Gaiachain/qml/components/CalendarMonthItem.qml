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

    property int fontCapitalization: Font.MixedCase

    signal dayClicked(date dayDate)

    ColumnLayout {
        id: topColumn
        anchors.fill: parent
        spacing: s(GStyle.tinySpacing)

        DayOfWeekRow {
            id: namesRow
            Layout.preferredWidth: grid.width
            Layout.alignment: Qt.AlignHCenter

            locale: grid.locale

            delegate: Items.GText {
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                font.pixelSize: s(GStyle.calendarNamePixelSize)
                font.capitalization: fontCapitalization
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

            spacing: GStyle.none

            onClicked: top.dayClicked(date)

            Component.onCompleted: {
                grid.minHeight = (s(GStyle.calendarDayMinHeight) + spacing) * grid.contentItem.rows
                grid.minWidth = (s(GStyle.calendarDayMinHeight) + spacing) * grid.contentItem.columns
                grid.scaleRatio = grid.contentItem.columns / grid.contentItem.rows
            }
        }
    }
}

