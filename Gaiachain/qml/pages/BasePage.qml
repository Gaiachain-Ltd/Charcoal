import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

import Qt.labs.calendar 1.0

import "../items" as Items

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

Item {
    id: top
    property bool footerVisible: true
    property bool headerVisible: true

    property int page: Enums.Page.InvalidPage

    property color backgroundColor: Style.backgroundColor

    default property alias content: pageContent.data

    function getMonthName(month) {
        switch(month) {
        case Calendar.January:return Strings.january
        case Calendar.February: return Strings.february
        case Calendar.March: return Strings.march
        case Calendar.April: return Strings.april
        case Calendar.May: return Strings.may
        case Calendar.June: return Strings.june
        case Calendar.July: return Strings.july
        case Calendar.August: return Strings.august
        case Calendar.September: return Strings.september
        case Calendar.October: return Strings.october
        case Calendar.November: return Strings.november
        case Calendar.December: return Strings.december
        default:
            console.warn("CalendarMonthItem: Invalid month provided!")
        }

        return Strings.january +"!" // Add "!" if invalid
    }

    ColumnLayout {
        id:  columnLayout
        anchors.fill: parent

        spacing: 0

        Items.NavigationHeader {
            Layout.fillWidth: true
            Layout.preferredHeight: s(Style.headerHeight)
            visible: top.headerVisible
        }

        Item {
            id: pageContent
            Layout.fillWidth: true
            Layout.fillHeight: true

            Rectangle {
                id: background
                anchors.fill: parent
                color: top.backgroundColor
            }
        }

        Items.Footer {
            Layout.fillWidth: true
            Layout.preferredHeight: s(Style.footerHeight)
            visible: top.footerVisible
        }
    }
}
