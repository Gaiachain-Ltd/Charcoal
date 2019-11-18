import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

BasePage {
    id: top

    property var packageData

    function refreshData() {
        // called from BasePage
        dataManager.fetchEventData(packageData.id, packageData.type)
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

            source: urlToDetails(Number(top.packageData.type))
            onLoaded: {
                item.packageData = top.packageData
                detailsFlickable.contentHeight = Qt.binding( function() {
                    return item.implicitHeight
                })
            }
        }
    }
}
