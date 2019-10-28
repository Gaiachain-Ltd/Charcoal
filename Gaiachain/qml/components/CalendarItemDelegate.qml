import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items

Item {
    id: top
    property int currentMonth: model.month
    readonly property bool isCurrentMonth: model.month === currentMonth

    implicitHeight: s(Style.calendarDayMinHeight)
    implicitWidth: s(Style.calendarDayMinHeight)

    Rectangle { // today border
        anchors.centerIn: parent
        width: s(Style.calendarDayMinHeight)
        height: width

        visible: model.today && isCurrentMonth

        radius: s(Style.tinyMargin)
        border {
            width: s(Style.calendarCurrentDayBorderWidth)
            color: Style.inputBorderColor
        }
        color: "transparent"
    }

    Column {
        anchors.centerIn: parent

        Items.BasicText {
            id: numberText
            anchors.horizontalCenter: parent.horizontalCenter

            verticalAlignment: Text.AlignTop

            text: model.day
            font.pixelSize: s(Style.calendarNumberPixelSize)
            color: top.isCurrentMonth ? Style.textPrimaryColor : Style.textDisabledColor
        }

        ListModel {
            id: packageTypesModel

            function initData() {
                var availablePackageTypes = DataGlobals.availablePackageTypes;
                for (var idx = 0; idx < availablePackageTypes.length; ++idx) {
                    var packageType = availablePackageTypes[idx]
                    append({ "packageType": packageType,
                               "typeColor": String(Helpers.packageTypeColor(packageType)),
                               "hasEvents": false })
                }
            }

            function updateData() {
                var datePackageTypes = packagesCalendarModel.datePackageTypesQml(model.date);

                var availablePackageTypes = DataGlobals.availablePackageTypes
                for (var idx = 0; idx < availablePackageTypes.length; ++idx) {
                    var packageType = availablePackageTypes[idx]
                    get(idx).hasEvents = datePackageTypes.includes(packageType)
                }
            }

            Component.onCompleted: {
                initData()
                updateData()
            }
        }

        Connections {
            target: packagesCalendarModel
            onDatePackageTypesChanged: {
                if ( (date.getFullYear() === model.year)
                        && (date.getMonth() === model.month)
                        && (date.getDate() === model.day) ) {
                    packageTypesModel.updateData()
                }
            }
        }

        Connections {
            target: model
            onMonthChanged: packageTypesModel.updateData()
            onYearChanged: packageTypesModel.updateData()
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter

            visible: top.isCurrentMonth
            spacing: s(Style.calendarDotSize) / 3

            Items.LayoutSpacer { height: s(Style.calendarDotSize); width: 1 }

            Repeater {
                model: packageTypesModel

                Rectangle {
                    width: visible ? s(Style.calendarDotSize) : 0
                    height: width
                    radius: width * 0.5

                    color: typeColor
                    visible: hasEvents
                }
            }

            Items.LayoutSpacer { height: s(Style.calendarDotSize); width: 1 }
        }
    }
}
