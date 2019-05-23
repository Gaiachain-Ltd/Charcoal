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

    property color circleColor: Style.currentCommodityColor
    property int circleSize: s(Style.calendarBigDotSize)

    signal titleClicked()
    signal dateClicked(date d)

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
                property bool hasEvents: false

                function updateData() {
                    hasEvents = calendarRangeProxyModel.hasEvents(myDate);
                }

                Component.onCompleted: updateData();
                Connections {
                    target: calendarRangeProxyModel
                    onEventsCommoditiesChanged: {
                        if (date.getFullYear() === myDate.getFullYear()) {
                            if (date.getMonth() === myDate.getMonth()) {
                                if (date.getDate() === myDate.getDate())
                                    updateData()
                            }
                        }
                    }
                }

                spacing: daySpacing

                BasicText {
                    anchors.horizontalCenter: parent.horizontalCenter

                    text: model.day
                    verticalAlignment: Text.AlignTop

                    opacity: currentMonth ? (utility.isWeekend(model.date) ? 0.4 : 1) : 0.05 // TO_DO should be in Style!
                    font: grid.font
                }

                Rectangle {
                    anchors.horizontalCenter: parent.horizontalCenter

                    width: circleSize
                    height: width
                    radius: width * 0.5

                    color: {
                        return currentMonth && hasEvents
                                ? circleColor
                                : "transparent"
                    }
                }
            }
        }

        LayoutSpacer { spacerHeight: top.bottomSpacing }
    }
}

