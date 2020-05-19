import QtQuick 2.11
import QtQuick.Layouts 1.11

import Qt.labs.calendar 1.0

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items

ColumnLayout {
    id: top

    property alias delegate: grid.delegate
    property alias gridItem: grid

    property date currentDate

    // Minimal selectable date
    property date bottomDate: new Date(2020, 04, 18)

    property int currentMonth: currentDate.getMonth()
    property int currentYear: currentDate.getFullYear()

    property int fontCapitalization: Font.MixedCase
    property color dayRowFontColor: GStyle.textPrimaryColor

    signal dayClicked(date dayDate)

    spacing: s(GStyle.tinySpacing)

    DayOfWeekRow {
        id: namesRow
        Layout.alignment: Qt.AlignHCenter
        Layout.fillWidth: true

        locale: grid.locale

        delegate: Items.GText {
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            font.pixelSize: s(GStyle.calendarNamePixelSize)
            font.capitalization: fontCapitalization
            text: Helper.formatShortName(model.shortName)
            color: dayRowFontColor
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

        Layout.minimumHeight: minHeight
        Layout.minimumWidth: minWidth
        Layout.fillWidth: true
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

