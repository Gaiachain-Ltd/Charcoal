import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items
import "../components" as Components

Components.CalendarItem {
    id: top

    implicitHeight: s(Style.calendarDayMinHeight)
    implicitWidth: s(Style.calendarDayMinHeight)

    ListModel {
        id: packageTypesModel

        function initData() {
            var availablePackageTypes = DataGlobals.availablePackageTypes;
            for (var idx = 0; idx < availablePackageTypes.length; ++idx) {
                var packageType = availablePackageTypes[idx]
                append({ "packageType": packageType,
                         "typeColor": String(Helper.packageTypeColor(packageType)),
                         "hasEvents": false })
            }
        }

        function updateData() {
            var datePackageTypes = packagesCalendarMonthModel.datePackageTypesQml(model.date);

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
        target: packagesCalendarMonthModel
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

    additionalContent: Row {
        Layout.alignment: Qt.AlignHCenter

        opacity: top.isCurrentMonth ? Style.visible : Style.hidden
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

