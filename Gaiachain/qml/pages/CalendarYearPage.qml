import QtQuick 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../components" as Components

BasePage {
    id: top

    title: Strings.calendar

    ListModel {
        id: dateModel

        function initData() {
           var currentYear = (new Date).getFullYear()

            for(var year = currentYear; year >= Style.calendarYearStartYear; --year) {
                append({"dateYear": year})
            }
        }

        Component.onCompleted: {
            initData()
        }
    }

    ListView {
        anchors {
            fill: parent
            margins: s(Style.hugeMargin)
        }
        spacing: s(Style.hugeMargin)
        clip: true

        model: dateModel

        delegate: Components.CalendarYearItem {
            width: parent.width

            year: dateYear

            onDateClicked: {
                var day = (new Date).getDate()
                pageManager.enter(Enums.Page.Calendar, {
                                         "currentDay": day,
                                         "currentMonth": month,
                                         "currentYear": year })
            }
        }
    }
}
