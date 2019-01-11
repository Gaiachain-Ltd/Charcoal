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
    property bool timberEnabled: commodityProxy.commodityEnabled(Enums.CommodityType.Timber) //TO_DO_LATER

    signal titleClicked()
    signal dateClicked(date d)

    property bool invalidateDelegates: false
    Connections {
        target: commodityRangeProxy
        onFilteringFinished: invalidateDelegates = !invalidateDelegates
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
                property date myDate: new Date(model.year, model.month, model.day)

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

                        return currentMonth && timberEnabled
                                && commodityRangeProxy.isEventToday(myDate)
                                ? circleColor: "transparent"
                    }
                }
            }
        }

        LayoutSpacer { spacerHeight: top.bottomSpacing }
    }
}

