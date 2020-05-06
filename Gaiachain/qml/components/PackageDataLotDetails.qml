import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.types 1.0

import "../items" as Items
import "../headers" as Headers
import "../components" as Components

Components.PackageDataDetails {
    id: top

    relatedPackagesModel: relatedPackagesModel

    function updatePackageData() {
        if (typeof(top.packageData) === "undefined") {
            return
        }

        var transporterId = packageData.properties[PackageDataProperties.TransporterId]
        var transporterName = ModelHelper.findItem("id", transporterId, "name", transportersModel)

        var destinationId = packageData.properties[PackageDataProperties.DestinationId]
        var destinationName = ModelHelper.findItem("id", destinationId, "name", destinationsModel)

        const data = [
           { "headerValue": Strings.organicCocoaTransporter,    "inputValue": Helper.minusIfNotDefined(transporterName) },
           { "headerValue": Strings.destination,                "inputValue": Helper.minusIfNotDefined(destinationName) },
           { "headerValue": Strings.transportDate,              "inputValue": Helper.minusIfNotDate(packageData.properties[PackageDataProperties.TransportDate]) }
       ]

        detailsModel.clear()
        for (var i = 0; i < data.length; ++i) {
            detailsModel.append(data[i])
        }

        var relatedPackages = packageData.relatedPackages

        relatedPackagesModel.clear()
        for (i = 0; i < relatedPackages.length; ++i) {
            relatedPackagesModel.append({ "headerValue": Strings.empty,
                                          "inputValue": relatedPackages[i],
                                          "inputSuffixValue": Strings.empty })
        }
    }

    onPackageDataChanged: updatePackageData()

    ListModel {
        id: detailsModel
    }

    ListModel {
        id: relatedPackagesModel
    }

    Repeater {
        model: detailsModel

        Headers.InputHeader {
            Layout.fillWidth: true

            readOnly: true
            color: GStyle.headerBackgroundColor
            headerText: headerValue
            inputText: inputValue
        }
    }
}
