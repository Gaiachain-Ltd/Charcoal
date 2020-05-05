import QtQuick 2.11
import QtQuick.Layouts 1.3

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items

Item {
    id: top

    implicitHeight: topRow.implicitHeight
    implicitWidth: topRow.implicitWidth

    ListModel {
        id: packageTypeActionsModel

        function initData() {
            var availablePackageTypes = DataGlobals.availablePackageTypes
            for (var idx = 0; idx < availablePackageTypes.length; ++idx) {
                var packageType = availablePackageTypes[idx]
                append({ "packageType": packageType,
                           "typeName": Helper.packageTypeName(packageType),
                           "typeColor": String(Helper.packageTypeColor(packageType)),
                           "eventsCount": 0 })
            }
        }

        function updateData() {
            var packageTypeEvents = packagesCalendarMonthModel.packageTypeEventsQml();

            var availablePackageTypes = DataGlobals.availablePackageTypes
            for (var idx = 0; idx < availablePackageTypes.length; ++idx) {
                var packageType = availablePackageTypes[idx]
                get(idx).eventsCount = packageTypeEvents[String(packageType)]
            }
        }

        Component.onCompleted: {
            initData()
            updateData()
        }
    }

    Connections {
        target: packagesCalendarMonthModel
        onPackageTypeEventsChanged: {
            packageTypeActionsModel.updateData()
        }
    }

    RowLayout {
        id: topRow
        anchors {
            fill: parent
            leftMargin: s(GStyle.bigMargin)
            rightMargin: s(GStyle.bigMargin)
        }

        Repeater {
            id: packagesFilteringRepeater
            model: packageTypeActionsModel

           Items.RectCheckBox {
               Layout.fillWidth: true
               Layout.preferredWidth: GStyle.none

               enabled: false

               elide: Text.ElideMiddle
               checked: true
               checkedColor: typeColor

               text: "%1 (%2)".arg(typeName).arg(eventsCount)
           }
        }
    }
}
