import QtQuick 2.12
import QtQuick.Layouts 1.11

import Qt.labs.calendar 1.0

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items
import "../components" as Components

BasePage {
    id: top
    title: Strings.calendar

    default property alias additionalContent: topLayout.data
    property alias calendarWidgets: topLayout.data

    readonly property var monthModel: [
        Calendar.January, Calendar.February, Calendar.March,
        Calendar.April, Calendar.May, Calendar.June,
        Calendar.July, Calendar.August, Calendar.September,
        Calendar.October, Calendar.November, Calendar.December
    ]

    property int currentDay
    property int currentMonth
    property int currentYear

    signal monthHeaderClicked()

    function previousMonth() {
        if (currentMonth === Calendar.January) {
            currentMonth = Calendar.December
            --currentYear
        } else {
            --currentMonth
        }
    }
    function nextMonth() {
        if (currentMonth === Calendar.December) {
            currentMonth = Calendar.January
            ++currentYear
        } else {
            ++currentMonth
        }
    }

    Component.onCompleted: updateModelRange()
    onCurrentDayChanged: updateModelRange()
    onCurrentMonthChanged: updateModelRange()
    onCurrentYearChanged: updateModelRange()

    ColumnLayout {
        id: topLayout
        anchors.fill: parent

        spacing: s(Style.bigMargin)

        ColumnLayout {
            Layout.margins: s(2 * Style.bigMargin)
            Layout.bottomMargin: 0

            spacing: s(Style.normalMargin)

            RowLayout {
                Layout.fillHeight: false

                spacing: s(Style.smallMargin)

                Items.BasicText {
                    id: monthYearText
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignVCenter

                    horizontalAlignment: Text.AlignLeft
                    text: Helpers.getMonthName(currentMonth) + " " + currentYear
                    font.weight: Font.DemiBold
                    font.pixelSize: s(Style.subtitlePixelSize)

                    MouseArea {
                        anchors.fill: parent
                        onClicked: top.monthHeaderClicked()
                    }
                }

                Items.LayoutSpacer { preferredHeight: top.bottomSpacing }

                Items.ImageButton {
                    Layout.preferredHeight: s(Style.buttonImageSmallHeight)
                    Layout.preferredWidth: height

                    palette.button: Style.calendarArrowButtonColor
                    source: Style.leftArrowImgUrl
                    padding: s(Style.tinyMargin)

                    onClicked: previousMonth()
                }
                Items.ImageButton {
                    Layout.preferredHeight: s(Style.buttonImageSmallHeight)
                    Layout.preferredWidth: height

                    palette.button: Style.calendarArrowButtonColor
                    source: Style.rightArrowImgUrl
                    padding: s(Style.tinyMargin)

                    onClicked: nextMonth()
                }
            }

            // calendar widgets goes here
        }

        // additional content goes here
    }
}
