import QtQuick 2.12
import QtQuick.Layouts 1.11
import QtQml.Models 2.12

import Qt.labs.calendar 1.0

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items

BasePage {
    id: top


    property date currentDate: new Date()
    property int currentYear: currentDate.getFullYear()
    property int currentMonth: monthModel[currentDate.getMonth()]
    property int displayedYear: currentYear
    property int lowestYear: currentYear

    readonly property var monthModel: [
        Calendar.January, Calendar.February, Calendar.March,
        Calendar.April, Calendar.May, Calendar.June,
        Calendar.July, Calendar.August, Calendar.September,
        Calendar.October, Calendar.November, Calendar.December
    ]

    onLowestYearChanged: {
        var startDate = new Date(lowestYear, 0, 1)
        var endDate = currentDate
        calendarRangeProxyModel.setDateTimeRange(startDate, endDate)
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

            datesModel.append({ "month": monthModel[i], "year": lowestYear })
        }
    }

    ListModel {
        id: datesModel
        Component.onCompleted: {
            addYear(currentMonth % 2 == 0 ? currentMonth + 1 : currentMonth)
            addYear()
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

            text: displayedYear
            horizontalAlignment: Text.AlignLeft

            font {
                pixelSize: s(60)
                bold: true
            }
        }

        GridView {
            id: grid

            readonly property int synchronousItems: 6 // 1.5 page loaded synchronously
            property int loadedItems: synchronousItems + 1

            Layout.fillHeight: true
            Layout.fillWidth: true

            clip: true

            cacheBuffer: top.height * 3

            cellHeight: height / 2
            cellWidth: width / 2

            model: datesModel

            verticalLayoutDirection: GridView.BottomToTop
            layoutDirection: Qt.RightToLeft

            onContentYChanged: {
                // Update curent year based on item at bottomLeft position.
                var bottomLeft = itemAt(contentX + cellWidth / 2,
                                        contentY + height - cellHeight * 0.3)
                if (bottomLeft !== null) {
                    top.displayedYear = bottomLeft.cYear

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

                enabled: year < currentYear || month <= currentMonth
                visible: enabled

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: s(20)

                    spacing: s(20)

                    Items.BasicText {
                        Layout.fillWidth: true

                        text: Helpers.getMonthName(month)
                        horizontalAlignment: Text.AlignLeft
                    }

                    Item {
                        id: asyncMonthItem
                        property bool synchronous: index <= grid.synchronousItems
                        property bool active: index <= grid.loadedItems
                        property bool ready: loader.status === Loader.Ready

                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        onReadyChanged: {
                            if (index == grid.loadedItems)
                                grid.loadedItems++;
                        }

                        Component {
                            id: monthItemComponent

                            Items.CalendarMonthItem {
                                anchors.fill: parent
                                currentMonth: month
                                currentYear: year
                                circleColor: Style.buttonBlackGreyColor
                                circleSize: s(Style.calendarSmallDotSize)

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: enterCalendarMonthPage(month, year)
                                }
                            }
                        }

                        Loader {
                            id: loader
                            anchors.fill: parent

                            asynchronous: false //!asyncMonthItem.synchronous
                            sourceComponent: monthItemComponent

                            visible: asyncMonthItem.ready
                            active: enabled && asyncMonthItem.active
                        }
                        Items.WaitOverlay {
                            anchors.fill: parent
                            visible: !asyncMonthItem.ready
                        }
                    }
                }
            }

            Component.onCompleted: {
                // Slow down flick as page component is quite heavy which leads to poor UX.
                flickDeceleration = flickDeceleration * 0.5
                maximumFlickVelocity = maximumFlickVelocity * 0.3
            }
        }
    }
}
