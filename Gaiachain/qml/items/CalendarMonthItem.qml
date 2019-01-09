import QtQuick 2.11
import QtQuick.Layouts 1.11

import Qt.labs.calendar 1.0

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

Item {
    id: top

    property int currentMonth: Calendar.December
    property int currentYear: 2018

    property int bottomSpacing: 0
    property alias dayFont: grid.font

    signal titleClicked()
    signal dateClicked(date d)

    // Month should be 0-indexed
    function getNextMonth (date, year, month) {
        if (date.getMonth() === 11) {
            return new Date(date.getFullYear() + 1, 0, 1);
        } else {
            return new Date(date.getFullYear(), date.getMonth() + 1, 1);
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        MonthGrid {
            id: grid

            Layout.fillWidth: true
            Layout.fillHeight: true

            month: currentMonth
            year: currentYear
            locale: Qt.locale("en_GB")
            font.pixelSize: s(25)

            onClicked: dateClicked(date)

            delegate: Column {
                property bool currentMonth: model.month === grid.month
                BasicText {
                    anchors.horizontalCenter: parent.horizontalCenter

                    text: model.day
                    verticalAlignment: Text.AlignTop

                    opacity: currentMonth ? 1 : 0.2
                    font: grid.font
                }

                Rectangle {
                    anchors.horizontalCenter: parent.horizontalCenter

                    width: parent.width / 4
                    height: width
                    radius: width / 2

                    color: {
                        var delegateDate = new Date(model.year, model.month, model.day)
                        var isEventToday = commodityRangeProxy.isEventToday(delegateDate)
                        var color = "gray"
                        if (commodityProxy.commodityEnabled(Enums.CommodityType.Timber))
                            color = "green"

                        console.log("Is event today", isEventToday, delegateDate)

                        return currentMonth ? color : "transparent"
                    }
                }
            }
        }

        LayoutSpacer { spacerHeight: top.bottomSpacing }
    }

    Component.onCompleted: {
        var startDate = new Date(currentYear, currentMonth, 1)
        var endDate = getNextMonth(startDate)

        console.log("Current start/end date", startDate, endDate)
        commodityProxy.setCommodityType(Enums.CommodityType.Timber)
        commodityRangeProxy.setDateTimeRange(startDate, endDate)

    }
}

