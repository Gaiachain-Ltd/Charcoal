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

    property int daySpacing: 0

    property color circleColor: Style.textGreenColor

    signal titleClicked()
    signal dateClicked(date d)

    property bool invalidateDelegates: false
    Connections {
        target: commodityRangeProxy
        onFilteringFinished: {
            invalidateDelegates = !invalidateDelegates
            console.log("Delegates invalidated", invalidateDelegates)
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
            font.family: Style.primaryFontFamily

            onClicked: dateClicked(date)

            delegate: Column {
                property bool currentMonth: model.month === grid.month
                spacing: daySpacing
                BasicText {
                    anchors.horizontalCenter: parent.horizontalCenter

                    text: model.day
                    verticalAlignment: Text.AlignTop

                    opacity: currentMonth ? (utility.isWeekend(model.date) ? 0.4 : 1) : 0.05
                    font: grid.font
                }

                Rectangle {
                    anchors.horizontalCenter: parent.horizontalCenter

                    width: parent.width * 0.2
                    height: width
                    radius: width * 0.5

                    color: {
                         // Used here to force change color after commodity type change
                        var invalidateColor = invalidateDelegates

                        var delegateDate = new Date(model.year, model.month, model.day)
                        var isEventToday = commodityRangeProxy.isEventToday(delegateDate)

                        var commodityTimberEnabled = commodityProxy.commodityEnabled(Enums.CommodityType.Timber)
                        var commodityCocoaEnabled = commodityProxy.commodityEnabled(Enums.CommodityType.Cocoa)

                        var colorUse = circleColor
                        if (commodityCocoaEnabled) {
                            colorUse = "brown"
                        }

                        console.log("Delegate commodity", isEventToday, commodityCocoaEnabled, commodityTimberEnabled)

                        return currentMonth && (commodityTimberEnabled || commodityCocoaEnabled) && isEventToday ? colorUse : "transparent"
                    }
                }
            }
        }

        LayoutSpacer { spacerHeight: top.bottomSpacing }
    }

    Timer {
        interval: 2000
        running: true
        onTriggered: {
            console.log("Timer trigered", invalidateDelegates)
            commodityProxy.setCommodityType(Enums.CommodityType.Cocoa, true)
        }
    }
}

