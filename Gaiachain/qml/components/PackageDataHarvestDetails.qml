import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.types 1.0

import "../items" as Items
import "../components" as Components

Components.PackageDataDetails {
    id: top

    Component.onCompleted: parcelsModel.allProducers = true
    Component.onDestruction: parcelsModel.allProducers = false

    function updatePackageData() {
        if (typeof(top.packageData) === "undefined") {
            return
        }

        var parcelId = packageData.properties[PackageDataProperties.ParcelId]
        var parcelData = ModelHelper.findItem("id", parcelId, ["code", "producerId"], parcelsModel)

        var producerId = parcelData[1]
        var producerData = ModelHelper.findItem("id", producerId, ["name", "code", "village"], producersModel)

        const data = [
           { "headerValue": Strings.nameOfProducer,       "inputValue": Helper.minusIfNotDefined(producerData[0]) },
           { "headerValue": Strings.producerIdNumber,     "inputValue": Helper.minusIfNotDefined(producerData[1]) },
           { "headerValue": Strings.village,              "inputValue": Helper.minusIfNotDefined(producerData[2]) },
           { "headerValue": Strings.parcelCode,           "inputValue": Helper.minusIfNotDefined(parcelData[0]) },
           { "headerValue": Strings.harvestDate,          "inputValue": Helper.minusIfNotTimestamp(packageData.properties[PackageDataProperties.HarvestDate]) },
           { "headerValue": Strings.breakingDate,         "inputValue": Helper.minusIfNotTimestamp(packageData.properties[PackageDataProperties.BreakingDate]) },
           { "headerValue": Strings.estimatedBeansVolume, "inputValue": Strings.kg.arg(Helper.minusIfNotDefinedOrZero(packageData.properties[PackageDataProperties.EstimatedBeansVolume])) }
       ]

        detailsModel.clear()
        for (var i = 0; i < data.length; ++i) {
            detailsModel.append(data[i])
        }
    }

    onPackageDataChanged: updatePackageData()

    ListModel {
        id: detailsModel
    }

    Repeater {
        model: detailsModel

        Items.InputHeader {
            Layout.fillWidth: true

            readOnly: true
            color: Style.headerBackgroundColor
            headerText: headerValue
            inputText: inputValue
        }
    }
}
