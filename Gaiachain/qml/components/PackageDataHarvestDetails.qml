import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.modelhelper 1.0
import com.gaiachain.packagedata 1.0

import "../items" as Items
import "../components" as Components

Components.PackageDataDetails {
    id: top

    Component.onCompleted: parcelsModel.allProducers = true
    Component.onDestruction: parcelsModel.allProducers = false

    onPackageDataChanged: {
        if (typeof(top.packageData) === "undefined") {
            return
        }

        var parcelId = packageData.properties[PackageDataProperties.ParcelId]
        var parcelData = ModelHelper.findItem("id", parcelId, ["code", "producerId"], parcelsModel)

        var producerId = parcelData[1]
        var producerData = ModelHelper.findItem("id", producerId, ["name", "code", "village"], producersModel)

        const data = [
           { "headerValue": Strings.nameOfProducer,       "inputValue": emptyIfNotDefined(producerData[0]) },
           { "headerValue": Strings.producerIdNumber,     "inputValue": emptyIfNotDefined(producerData[1]) },
           { "headerValue": Strings.village,              "inputValue": emptyIfNotDefined(producerData[2]) },
           { "headerValue": Strings.parcelCode,           "inputValue": emptyIfNotDefined(parcelData[0]) },
           { "headerValue": Strings.harvestDate,          "inputValue": emptyIfNotDate(packageData.properties[PackageDataProperties.HarvestDate]) },
           { "headerValue": Strings.breakingDate,         "inputValue": emptyIfNotDate(packageData.properties[PackageDataProperties.BreakingDate]) },
           { "headerValue": Strings.estimatedBeansVolume, "inputValue": Strings.kg.arg(minusIfNotDefinedOrZero(packageData.properties[PackageDataProperties.EstimatedBeansVolume])) }
       ]

        detailsModel.clear()
        for (var i = 0; i < data.length; ++i) {
            detailsModel.append(data[i])
        }
    }

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
