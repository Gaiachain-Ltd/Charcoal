import QtQuick 2.11
import QtQuick.Layouts 1.3

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items

Item {
    id: top
    implicitHeight: topColumn.implicitHeight
    implicitWidth: topColumn.implicitWidth

    ListModel {
        id: packageTypeActionsModel

        function initData() {
            var availablePackageTypes = DataGlobals.availablePackageTypes
            for (var idx = 0; idx < availablePackageTypes.length; ++idx) {
                var packageType = availablePackageTypes[idx]
                append({ "packageType": packageType,
                           "typeName": Helpers.packageTypeName(packageType),
                           "typeColor": String(Helpers.packageTypeColor(packageType)),
                           "eventsCount": 0 })
            }
        }

        function updateData() {
            var packageTypeEvents = packageTypesCompanyCalendarModel.packageTypeEventsQml();

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
        target: packageTypesCompanyCalendarModel
        onPackageTypeEventsChanged: {
            packageTypeActionsModel.updateData()
        }
    }

    Column {
        id: topColumn
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        Repeater {
            model: packageTypeActionsModel

            Rectangle {
                id: barTop
                property int verticalPadding: s(Style.middleMargin);
                property int horizontalPadding: s(Style.hugeMargin);

                width: parent.width
                height: layout.implicitHeight + 2 * barTop.verticalPadding

                color: typeColor

                RowLayout {
                    id: layout
                    anchors{
                        fill: parent
                        topMargin: barTop.verticalPadding
                        bottomMargin: barTop.verticalPadding
                        leftMargin: barTop.horizontalPadding
                        rightMargin: barTop.horizontalPadding
                    }

                    Items.BasicText {
                        text: typeName

                        font.weight: Font.DemiBold
                        color: Style.textSecondaryColor
                    }

                    Items.LayoutSpacer { horizontal: true }

                    Items.BasicText {
                        text: eventsCount + " " + Strings.activities

                        color: Style.textSecondaryColor
                    }
                }
            }
        }
    }
}
