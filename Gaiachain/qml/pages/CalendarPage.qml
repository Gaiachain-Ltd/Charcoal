import QtQuick 2.12
import QtQuick.Layouts 1.11
import QtQml.Models 2.12

import Qt.labs.calendar 1.0

import com.gaiachain.enums 1.0

import "../items" as Items

BasePage {
    id: top

    bottomToolbarVisible: true

    property int currentYear: 2018
    property int lowestYear: currentYear
    property int currentMonth: Calendar.December

    readonly property var monthModel: [
        Calendar.January, Calendar.February, Calendar.March,
        Calendar.April, Calendar.May, Calendar.June,
        Calendar.July, Calendar.August, Calendar.September,
        Calendar.October, Calendar.November, Calendar.December
    ]

    // TO_DO fix model order
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

    ListModel { //Add model in cpp T0_DO
        id: datesModel
        Component.onCompleted: {
            addYear(currentYear, currentMonth)
            grid.positionViewAtEnd()
        }
    }

    function enterCalendarMonthPage(month, year) {
        console.log("Month/Year", month, year)
        pageManager.enterPage(Enums.Page.CalendarMonth, {"currentMonth": month, "currentYear": year})
    }

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
            id: grid
            Layout.fillHeight: true
            Layout.fillWidth: true

            clip: true

            cellHeight: parent.height / 2
            cellWidth: parent.width / 2

            model: datesModel

            onAtYBeginningChanged: {
                if (atYBeginning) {
                    lowestYear = lowestYear - 1
                    top.addYear(lowestYear)
                }
            }

            delegate: Item {
                width: GridView.view.cellWidth
                height: GridView.view.cellHeight

                Items.CalendarMonthItem {
                    anchors.fill: parent
                    anchors.margins: s(30)

                    currentMonth: month
                    currentYear: year

                    MouseArea {
                        anchors.fill: parent
                        onClicked: enterCalendarMonthPage(month, year)
                    }
                }
            }
        }
    }
}
