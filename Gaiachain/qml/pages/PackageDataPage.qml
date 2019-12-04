import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

BasePage {
    id: top

    property string packageId
    property int packageType
    property var packageData

    Component.onCompleted: refreshData()

    Connections {
        target: dataManager
        onPackageData: { top.packageData = packageData }
    }
    Connections {
        target: eventsModel
        onEntryInserted: {
            // update view if new data arrived
            if (entryData.at(0) === top.packageId) {
                refreshData();
            }
        }
    }

    function refreshData() {
        // called from BasePage
        dataManager.fetchEventData(packageId, packageType)
        dataManager.getPackageData(packageId)
    }

    function urlToDetails(packageType) {
        switch(packageType) {
        case Enums.PackageType.Harvest: return "qrc:/components/PackageDataHarvestDetails.qml"
        case Enums.PackageType.Sac: return "qrc:/components/PackageDataSacDetails.qml"
        case Enums.PackageType.Lot: return "qrc:/components/PackageDataLotDetails.qml"
        default:
            console.warn("packageTypeName: Invalid packageType provided!")
        }

        return Strings.empty
    }

    Flickable {
        id: detailsFlickable
        anchors {
            fill: parent
            margins: s(Style.bigMargin)
        }

        boundsBehavior: Flickable.StopAtBounds

        Loader {
            anchors.fill: parent

            source: urlToDetails(Number(top.packageType))
            onLoaded: {
                item.packageId = top.packageId
                item.packageType = top.packageType
                item.packageData = Qt.binding(function() { return top.packageData; })
                detailsFlickable.contentHeight = Qt.binding( function() {
                    return item.implicitHeight
                })
            }
        }
    }
}
