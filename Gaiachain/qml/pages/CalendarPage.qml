import QtQuick 2.12
import QtQuick.Layouts 1.11
import QtQml.Models 2.12

import Qt.labs.calendar 1.0

import com.gaiachain.enums 1.0

import "../items" as Items

BasePage {
    id: top

    property int currentYear: 2018 //TO_DO set current year and month to current one
    property int lowestYear: currentYear
    property int currentMonth: Calendar.December

    readonly property var monthModel: [
        Calendar.January, Calendar.February, Calendar.March,
        Calendar.April, Calendar.May, Calendar.June,
        Calendar.July, Calendar.August, Calendar.September,
        Calendar.October, Calendar.November, Calendar.December
    ]

    function addYear(lastMonth) {
        var endMonth = lastMonth === undefined ? Calendar.December : lastMonth
        var blockInsert = true // Block insert until lastMonth is found
        for (var i = monthModel.length-1; i >= 0; --i) {
            if (blockInsert) {
                if (monthModel[i] === endMonth) {
                    blockInsert = false
                } else {
                    continue
                }
            }

            datesModel.insert(0, { "month": monthModel[i], "year": lowestYear })
        }

        --lowestYear
    }

    ListModel {
        id: datesModel
        Component.onCompleted: {
            addYear(currentMonth)
            addYear()
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

        Items.BasicText {
            id: yearText

            Layout.fillWidth: true

            text: currentYear
            horizontalAlignment: Text.AlignLeft

            font.pixelSize: s(60)
            font.bold: true
        }

        GridView {
            id: grid
            Layout.fillHeight: true
            Layout.fillWidth: true

            clip: true

            cellHeight: height / 2
            cellWidth: width / 2

            model: datesModel

            onAtYBeginningChanged: {
                if (atYBeginning) {
                    // Add next year if we're at the begining
                    addYear()
                    grid.positionViewAtIndex(monthModel.length, GridView.Beginning)
                }
            }

            onContentYChanged: {
                // Update curent year based on item at (contentX, contentY) position
                top.currentYear = itemAt(contentX, contentY).cYear
            }


            delegate: Item {
                width: GridView.view.cellWidth
                height: GridView.view.cellHeight

                property var cYear: year

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: s(30)

                    spacing: s(20)

                    Items.BasicText {
                        Layout.fillWidth: true

                        text: getMonthName(month)
                        horizontalAlignment: Text.AlignLeft
                    }

                    Items.CalendarMonthItem {
                        Layout.fillHeight: true
                        Layout.fillWidth: true

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
}
