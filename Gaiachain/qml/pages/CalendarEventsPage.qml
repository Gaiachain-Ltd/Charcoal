import QtQuick 2.11
import QtQuick.Layouts 1.11

import Qt.labs.calendar 1.0

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

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

    onCurrentDateChanged: {
        var daysCount = daysInMonth(currentDate.getFullYear(), currentDate.getMonth())
        daysModel.clear()

        for (var i=1; i <= daysCount; ++i)
            daysModel.append({"day": i, "isWeekend": utility.isWeekend(new Date(currentDate.getFullYear(), currentDate.getMonth(), i))})

        // TO_DO fix postioing at start
        daysView.positionViewAtIndex(currentDate.getDate() - 1, ListView.Contain)
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

            text: top.getMonthName(top.currentMonth) + " " + top.currentYear
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

            onSelectedDayChanged: positionViewAtIndex(selectedDay-1, ListView.Contain)

            Layout.fillWidth: true
            Layout.preferredHeight: s(Style.bigMargin) * 3

            model: daysModel

            orientation: ListView.Horizontal
            clip: true

            delegate: Item {
                id: delegate
                height: ListView.view.height
                width: ListView.view.width / ListView.view.visibleCount

                property bool isSelected: ListView.view.selectedDay === day

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
                            color: delegate.isSelected ? "white" : "black"
                            opacity: isWeekend ? 0.3 : 1
                        }
                    }
                    Rectangle {
                        Layout.alignment: Qt.AlignHCenter
                        Layout.preferredHeight: s(20)
                        Layout.preferredWidth: s(20)
                        radius: width * 0.5
                        color: Style.textGreenColor
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
            backgroundColor: "#FFF5F5F5"

            onDelegateClicked: top.enterShipmentDetailsPage(data)

            viewModel: ListModel { //TO_DO
                ListElement {
                    name: "Apple"
                    cost: 2.45
                }
                ListElement {
                    name: "Orange"
                    cost: 3.25
                }
                ListElement {
                    name: "Banana"
                    cost: 1.95
                }
            }
        }
    }
}
