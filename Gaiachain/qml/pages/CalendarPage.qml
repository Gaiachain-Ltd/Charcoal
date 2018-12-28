import QtQuick 2.11
import QtQuick.Layouts 1.11
import QtQml.Models 2.12

import Qt.labs.calendar 1.0

import com.gaiachain.enums 1.0

import "../items" as Items

BasePage {
    id: top

    bottomToolbarVisible: true

    property int currentYear: 2018
    property int currentMonth: Calendar.December

    readonly property var monthModel: [
        Calendar.January, Calendar.February, Calendar.March,
        Calendar.April, Calendar.May, Calendar.June,
        Calendar.July, Calendar.August, Calendar.September,
        Calendar.October, Calendar.November, Calendar.December
    ]

    function addYear(newYear, lastMonth) {
        var endMonth = lastMonth === undefined ? Calendar.December : lastMonth
        for (var i=0; i < monthModel.length; ++i) {
            datesModel.append({
                                  "month": monthModel[i],
                                  "year": newYear
                              })

            if (monthModel[i] === lastMonth)
                break
        }
    }


//    ListModel {
//        id: datesModel
//        Component.onCompleted: addYear(currentYear, currentMonth)
//    }

    ColumnLayout {
        anchors {
            fill: parent
            margins: s(40)
        }

        spacing: s(40)

        Text {
            Layout.fillWidth: true

            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft

            text: currentYear
            font.pixelSize: s(60)
            font.bold: true
        }

        GridView {
            Layout.fillHeight: true
            Layout.fillWidth: true

            model: datesModel
            delegate: Items.CalendarMonthItem {
                currentMonth: month
                currentYear: year
            }
        }
    }
}
