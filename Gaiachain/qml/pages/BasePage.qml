import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

import Qt.labs.calendar 1.0

import "../items" as Items

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

Items.GenericPanel
{
    id: top
    property bool footerVisible: true
    property bool headerVisible: true

    property color backgroundColor: Style.pageBaseBackgroundColor

    property alias header: navigationHeader
    property alias mainOverlayVisible: mainOverlay.visible

    function closeEventHandler() {
        navigationHeader.backHandler() // calling back button
    }

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
            id: navigationHeader
            Layout.fillWidth: true
            Layout.topMargin: mainWindow.headerTopMargin
            Layout.preferredHeight: s(Style.headerHeight)
            visible: top.headerVisible
            z: 5
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            layer.enabled: true
            clip: true

            Rectangle {
                id: background
                anchors.fill: parent
                color: top.backgroundColor
            }

            Item {
                id: pageContent
                readonly property int verticalOffset: -Math.max(mainWindow.bottomMargin, mainWindow.bottomMarginKeyboard * 0.5)
                anchors {
                    left: parent.left
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                    verticalCenterOffset: pageContent.verticalOffset
                }
                height: parent.height - mainWindow.bottomMarginKeyboard * 0.5
            }
        }

        Items.Footer {
            Layout.fillWidth: true
            // Math.round added because line below footer would appear
            Layout.preferredHeight: Math.round(s(Style.footerHeight))
            visible: top.footerVisible
        }
    }

    Items.WaitOverlay {
        id: mainOverlay
        anchors.fill: parent
    }
}
