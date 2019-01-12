import QtQuick 2.12
import QtQuick.Layouts 1.11
import QtQml.Models 2.12

import Qt.labs.calendar 1.0

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

import "../items" as Items

BasePage {
    id: top

    property date currentDate: new Date()
    property int currentYear: currentDate.getFullYear()
    property int lowestYear: currentYear
    property int currentMonth: monthModel[currentDate.getMonth()]

    readonly property var monthModel: [
        Calendar.January, Calendar.February, Calendar.March,
        Calendar.April, Calendar.May, Calendar.June,
        Calendar.July, Calendar.August, Calendar.September,
        Calendar.October, Calendar.November, Calendar.December
    ]

    Connections {
        target: commodityRangeProxy
        onFilteringFinished: {
            overlayId.visible = false
        }
    }

    onLowestYearChanged: {
        overlayId.visible = true

        var startDate = new Date(lowestYear, 0, 1)
        var endDate = currentDate
        commodityRangeProxy.setDateTimeRange(startDate, endDate)
    }

    function addYear(lastMonth) {
        var endMonth = lastMonth

        /* For first year we want months up to currentDate
         * and we don't want to update lowestYear
         */
        if (endMonth === undefined) {
            endMonth = Calendar.December
            --lowestYear
        }

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
        pageManager.enter(Enums.Page.CalendarMonth, {"currentMonth": month, "currentYear": year})
    }

    ColumnLayout {
        anchors {
            fill: parent
            margins: s(20)
        }

        spacing: s(20)

        Items.BasicText {
            id: yearText

            Layout.fillWidth: true
            Layout.leftMargin: s(20)

            text: currentYear
            horizontalAlignment: Text.AlignLeft

            font {
                pixelSize: s(60)
                bold: true
            }
        }

        GridView {
            id: grid
            Layout.fillHeight: true
            Layout.fillWidth: true

            clip: true

            cacheBuffer: top.height * 3

            cellHeight: height / 2
            cellWidth: width / 2

            model: datesModel

            onContentYChanged: {
                // Update curent year based on item at bottomLeft position.
                var bottomLeft = itemAt(contentX + cellWidth / 2,
                                        contentY + height - cellHeight * 0.3)
                if (bottomLeft !== null) {
                    top.currentYear = bottomLeft.cYear

                    // Add next year when we're in May. If we set March here recurently next years are added...
                    if (bottomLeft.cMonth === Calendar.May && bottomLeft.cYear === lowestYear)
                         addYear()
                }
            }

            delegate: Item {
                width: GridView.view.cellWidth
                height: GridView.view.cellHeight

                property int cYear: year
                property int cMonth: month

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: s(20)

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
                        circleColor: Style.buttonBlackGreyColor

                        MouseArea {
                            anchors.fill: parent
                            onClicked: enterCalendarMonthPage(month, year)
                        }
                    }
                }
            }

            Items.WaitOverlay
            {
                id: overlayId

                anchors.fill: parent

                opacity: 0.7
                logoVisible: false
            }

            Component.onCompleted: {
                // Slow down flick as page component is quite heavy which leads to poor UX.
                flickDeceleration = flickDeceleration * 0.5
                maximumFlickVelocity = maximumFlickVelocity * 0.3
            }
        }
    }
}
