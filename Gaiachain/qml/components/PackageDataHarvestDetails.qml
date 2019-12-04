import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.packagedata 1.0

import "../items" as Items
import "../components" as Components

Components.PackageDataDetails {
    id: top

    onPackageDataChanged: {
        if (typeof(top.packageData) === "undefined") {
            return
        }

        const data = [
           { "headerValue": Strings.nameOfProducer,       "inputValue": emptyIfNotDefnied(packageData.properties[PackageDataProperties.ProducerName]) },
           { "headerValue": Strings.producerIdNumber,     "inputValue": emptyIfNotDefnied(packageData.properties[PackageDataProperties.ProducerId]) },
           { "headerValue": Strings.village,              "inputValue": emptyIfNotDefnied(packageData.properties[PackageDataProperties.Village]) },
           { "headerValue": Strings.parcelCode,           "inputValue": emptyIfNotDefnied(packageData.properties[PackageDataProperties.ParcelCode]) },
           { "headerValue": Strings.harvestDate,          "inputValue": emptyIfNotDate(packageData.properties[PackageDataProperties.HarvestDate]) },
           { "headerValue": Strings.breakingDate,         "inputValue": emptyIfNotDate(packageData.properties[PackageDataProperties.BreakingDate]) }
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
