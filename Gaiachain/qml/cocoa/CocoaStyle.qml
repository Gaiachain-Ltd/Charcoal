pragma Singleton

import QtQuick 2.12

import "../"

AbstractStyle {
    // PAGES SPECIFIC
    // login loading
    readonly property color loginLoadingTextColor: "#d6d6d6"

    // main menu
    readonly property int menuButtonHeight: 180
    readonly property int submenuButtonHeight: 125
    readonly property int menuButtonImageHeight: buttonImageBigHeight * 0.85
    readonly property int submenuButtonImageHeight: buttonImageSmallHeight
    readonly property real submenuButtonPixelSize: buttonPixelSize * 0.8
    readonly property color menuButtonColor: "#f6f6f6"

    // calendar pages
    readonly property color calendarArrowButtonColor: "#e8eaec"
    readonly property int calendarDotSize: 10
    readonly property int calendarCurrentDayBorderWidth: 3
    readonly property int calendarNamePixelSize: tinyPixelSize
    readonly property int calendarNumberPixelSize: smallPixelSize
    readonly property int calendarDayMinHeight: calendarNumberPixelSize + calendarDotSize + 2 * smallMargin
    readonly property int calendarDayChangeDuration: 1000
    readonly property int calendarMonthMinWidth: 150
    readonly property int calendarMonthMinHeight: 120
    readonly property int calendarYearStartYear: 2018
}
