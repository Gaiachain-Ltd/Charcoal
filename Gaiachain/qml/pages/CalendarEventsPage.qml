import QtQuick 2.11
import QtQuick.Layouts 1.11

import Qt.labs.calendar 1.0

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items

BasePage {
    id: top

    property date currentDate
    property int currentDay: currentDate.getDate()
    property int currentMonth: currentDate.getMonth()
    property int currentYear: currentDate.getFullYear()

    function enterEventDetailsPage(data) {
        pageManager.enterPage(Enums.Page.EventDetails, data)
    }

    // Month should be 0-indexed
    function daysInMonth (year, month) {
        return new Date(year, month + 1, 0).getDate();
    }

    onCurrentDateChanged: {
        var daysCount = daysInMonth(currentDate.getFullYear(), currentDate.getMonth())
        daysModel.clear()

        for (var i=1; i <= daysCount; ++i)
            daysModel.append({"day": i})

        // TO_DO fix postioing at start
        daysView.positionViewAtIndex(currentDate.getDate() - 1, ListView.Contain)
    }

    ListModel {
        id: daysModel
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: s(40)
        spacing: s(50)

        Items.ImageItem {
            Layout.fillWidth: true

            buttonHeight: s(30)
            buttonWidth: s(30)
            imageUrl: Style.backBlackImgUrl

            text: top.getMonthName(top.currentMonth) + " " + top.currentYear
            textFont.bold: true
            textFont.pixelSize: s(50)

            MouseArea {
                anchors.fill: parent
                onClicked: pageManager.popPage()
            }
        }

        ListView {
            id: daysView

            property int visibleCount: 7
            property int selectedDay: top.currentDay

            onSelectedDayChanged: positionViewAtIndex(selectedDay-1, ListView.Contain)

            Layout.fillWidth: true
            Layout.preferredHeight: s(100)

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
                    Rectangle {
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                        Layout.preferredHeight: parent.width
                        Layout.preferredWidth: parent.width
                        radius: width / 2
                        color: delegate.isSelected ? "gray" : "transparent"

                        Text {
                            anchors.centerIn: parent
                            text: day
                            color: delegate.isSelected ? "white" : "black"
                            font.pixelSize: s(40)
                        }
                    }
                    Items.LayoutSpacer {}
                    Rectangle {
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                        Layout.preferredHeight: s(20)
                        Layout.preferredWidth: s(20)
                        radius: width / 2
                        color: "green"
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

            onDelegateClicked: top.enterEventDetailsPage(data)

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
