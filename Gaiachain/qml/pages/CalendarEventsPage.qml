import QtQuick 2.11
import QtQuick.Layouts 1.11

import Qt.labs.calendar 1.0

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items

BasePage {
    id: top

    property string currentDateStr
    property date currentDate: utility.convertDateString(currentDateStr, utility.defaultDateFormat())
    property int currentDay: currentDate.getDate()
    property int currentMonth: currentDate.getMonth()
    property int currentYear: currentDate.getFullYear()

    function enterShipmentDetailsPage(data) {
        pageManager.enter(Enums.Page.ShipmentDetails, data)
    }

    // Month should be 0-indexed
    function daysInMonth (year, month) {
        return new Date(year, month + 1, 0).getDate();
    }

    onCurrentDayChanged: {
        var today = new Date(currentYear, currentMonth, currentDay)
        var tomorrow = new Date(today);
        tomorrow.setDate(today.getDate() + 1);

        dateEventsRangeProxyModel.setDateTimeRange(today, tomorrow)
    }

    onCurrentDateChanged: {
        var daysCount = daysInMonth(currentDate.getFullYear(), currentDate.getMonth())
        daysModel.clear()

        for (var i=1; i <= daysCount; ++i)
            daysModel.append({"day": i, "isWeekend": utility.isWeekend(new Date(currentDate.getFullYear(), currentDate.getMonth(), i))})
    }

    ListModel {
        id: daysModel
    }

    ColumnLayout {
        anchors {
            fill: parent
            topMargin: s(Style.bigMargin)
        }
        spacing: s(Style.bigMargin)

        Items.ImageItem {
            Layout.fillWidth: true
            Layout.leftMargin: s(Style.normalMargin)

            buttonHeight: s(30)
            buttonWidth: s(30)
            imageUrl: Style.backBlackImgUrl

            text: Helpers.getMonthName(top.currentMonth) + " " + top.currentYear
            textFont.bold: true
            textFont.pixelSize: s(50)

            MouseArea {
                anchors.fill: parent
                onClicked: pageManager.back()
            }
        }

        ListView {
            id: daysView

            property int visibleCount: 7
            property int selectedDay: top.currentDay
            property int cellWidth: width / visibleCount

            Layout.fillWidth: true
            Layout.preferredHeight: s(Style.bigMargin) * 3

            model: daysModel

            cacheBuffer: count * cellWidth

            orientation: ListView.Horizontal
            clip: true

            delegate: Item {
                id: delegate
                height: ListView.view.height
                width: ListView.view.cellWidth

                property bool isSelected: ListView.view.selectedDay === day
                property date myDate: new Date(currentYear, currentMonth, day)
                property bool hasEvents: false

                function updateData() {
                    hasEvents = calendarRangeProxyModel.hasEvents(myDate);
                }

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
                Component.onCompleted: {
                    updateData();

                    // TO_DO find a better solution (center!)
                    if (delegate.isSelected)
                        ListView.view.positionViewAtIndex(ListView.view.selectedDay-1, ListView.Center)
                }

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0
                    Rectangle {
                        Layout.alignment: Qt.AlignHCenter
                        Layout.preferredHeight: parent.width * 0.7
                        Layout.preferredWidth: parent.width * 0.7
                        radius: width * 0.5
                        color: delegate.isSelected ? Style.buttonBlackGreyColor : "transparent"

                        Items.BasicText {
                            anchors.centerIn: parent
                            text: day
                            color: delegate.isSelected ? "white" : "black" // TO_DO should be in Style!
                            opacity: isWeekend ? 0.3 : 1 // TO_DO should be in Style!
                        }
                    }
                    Rectangle {
                        Layout.alignment: Qt.AlignHCenter
                        Layout.preferredHeight: s(20)
                        Layout.preferredWidth: s(20)
                        radius: width * 0.5
                        color: hasEvents ? Style.currentCommodityColor : "transparent"
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: currentDay = day
                }
            }
        }

        Items.EventsListView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            delegateHeight: s(100)
            backgroundColor: "#FFF5F5F5"    // TO_DO should be in Style!

            onDelegateClicked: top.enterShipmentDetailsPage(data)

            viewModel: dateEventsRangeProxyModel

            displayDate: false
        }
    }
}
