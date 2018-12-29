import QtQuick 2.11
import QtQuick.Layouts 1.11

import Qt.labs.calendar 1.0

import com.gaiachain.style 1.0

Item {
    id: top

    property int currentMonth: Calendar.December
    property int currentYear: 2018

    property int bottomSpacing: 0
    property alias dayFont: grid.font

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

            onClicked: dateClicked(date)

            delegate: Column {
                property bool currentMonth: model.month === grid.month
                Text {
                    anchors.horizontalCenter: parent.horizontalCenter

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignTop
                    opacity: currentMonth ? 1 : 0.2
                    text: model.day
                    font: grid.font
                }

                Rectangle {
                    anchors.horizontalCenter: parent.horizontalCenter

                    width: parent.width / 4
                    height: width
                    radius: width / 2

                    color: currentMonth ? "gray" : "transparent"
                }
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: top.bottomSpacing
        }
    }
}

