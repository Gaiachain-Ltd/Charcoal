import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.enums 1.0

import "../items" as Items

Item {
    id: top

    default property alias additionalContent: topLayout.data
    property alias relatedPackagesModel: headerAndRelatedPackages.model

    property var packageData

    implicitWidth: topLayout.implicitWidth
    implicitHeight: topLayout.implicitHeight

    function packageTypeDetailsName(packageType) {
        switch(packageType) {
        case Enums.PackageType.Harvest: return Strings.details.arg(Strings.harvestId)
        case Enums.PackageType.Sac: return Strings.details.arg(Strings.sacId)
        case Enums.PackageType.Lot: return Strings.details.arg(Strings.lotId)
        default:
            console.warn("packageTypeName: Invalid packageType provided!")
        }

        return Strings.empty
    }

    ColumnLayout {
        id: topLayout

        anchors.fill: parent

        spacing: s(Style.smallMargin)

        Items.InputHeaderWithExpandList {
            id: headerAndRelatedPackages

            Layout.fillWidth: true

            color: String(Helpers.packageTypeColor(Number(packageData.type)))
            headerText: top.packageTypeDetailsName(Number(packageData.type))
            inputText: packageData.id
        }
    }
}