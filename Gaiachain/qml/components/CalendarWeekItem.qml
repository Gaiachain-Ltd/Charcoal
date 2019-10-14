import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items

Item {
    id: top
    implicitWidth: row.implicitWidth
    implicitHeight: row.implicitHeight

    property int currentDay
    property int currentMonth
    property int currentYear

    signal dayClicked(date dayDate)
    onDayClicked: currentDay = dayDate.getDate()

    Component {
        id: modelDataComponent
        QtObject {
            property date date
            readonly property int day: date.getDate()
            readonly property int month: date.getMonth()
            readonly property int year: date.getFullYear()
            readonly property bool today: day === top.currentDay
            readonly property int weekNumber: -1   // unsupported, not used in CalendarItemDelegate
        }
    }

    QtObject {
        id: d
        property bool initFinished: false

        function updateCurrentDay() {
            var daysCount = Helpers.daysInMonth(currentYear, currentMonth)
            if (currentDay > daysCount) {
                currentDay = daysCount
            }
        }

        function initDayNamesModel() {
            // use Locale instead of Qt days of week to keep js enumeration (Sunday = 0)
            for (var dayOfWeek=Locale.Sunday; dayOfWeek <= Locale.Saturday; ++dayOfWeek) {
                daysNamesModel.append({ "shortName": Qt.locale().dayName(dayOfWeek, Locale.ShortFormat) })
            }
        }


        function appendDay(day) {
            // structure of model need to fit with MonthGrid delegates model
            var object = modelDataComponent.createObject(daysModel,
                                                         { "date": new Date(currentYear, currentMonth, day) })
            daysModel.append({ "model": object })
        }

        function initDaysModel() {
            if (initFinished) {
                return
            }

            var daysCount = Helpers.daysInMonth(currentYear, currentMonth)
            for (var day=1; day <= daysCount; ++day) {
                appendDay(day)
            }
            row.currentIndex = Qt.binding(function() { return currentDay - 1 })

            initFinished = true
        }

        function updateDaysModel() {
            if (!initFinished) {
                return
            }

            var oldDaysCount = daysModel.count
            var daysCount = Helpers.daysInMonth(currentYear, currentMonth)

            // adjust days number
            if (oldDaysCount > daysCount) {
                daysModel.remove(daysCount, oldDaysCount - daysCount)
            } else if (oldDaysCount < daysCount) {
                for (var newDay = oldDaysCount + 1; newDay <= daysCount; ++newDay) {
                    appendDay(newDay)
                }
            }

            // update date
            for (var day = 1; day <= daysCount; ++day) {
                daysModel.get(day - 1).model.date = new Date(currentYear, currentMonth, day)
            }
        }

    }

    Component.onCompleted: { d.updateCurrentDay(); d.initDayNamesModel(); d.initDaysModel() }
    onCurrentMonthChanged: { d.updateCurrentDay(); d.updateDaysModel() }
    onCurrentYearChanged: { d.updateCurrentDay(); d.updateDaysModel() }

    ListModel {
        id: daysNamesModel
    }
    ListModel {
        id: daysModel
    }

    ListView {
        id: row
        anchors.fill: parent

        readonly property int visibleCount: 7
        readonly property int cellWidth: width / visibleCount

        function dayClickedHandler(dayDate) {
            top.dayClicked(dayDate)
        }

        model: daysModel

        preferredHighlightBegin: cellWidth * Math.floor(visibleCount / 2)
        preferredHighlightEnd: cellWidth * Math.ceil(visibleCount / 2)
        highlightRangeMode: ListView.StrictlyEnforceRange

        highlightMoveVelocity: -1   // use duration
        highlightMoveDuration: Style.calendarDayChangeDuration

        cacheBuffer: count * cellWidth
        orientation: ListView.Horizontal
        boundsBehavior: Flickable.OvershootBounds
        clip: true

        implicitHeight: currentItem.implicitHeight
        delegate: ColumnLayout {
            id: delegateTop
            spacing: s(Style.tinySpacing)

            Items.BasicText {
                Layout.fillWidth: true

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                font.pixelSize: s(Style.calendarNamePixelSize)
                text: daysNamesModel.get(model.date.getDay()).shortName
            }

            CalendarItemDelegate {
                Layout.preferredWidth: row.cellWidth
                Layout.preferredHeight: row.cellWidth

                MouseArea {
                    anchors.fill: parent
                    onClicked: delegateTop.ListView.view.dayClickedHandler(model.date)
                }
            }
        }
    }
}

