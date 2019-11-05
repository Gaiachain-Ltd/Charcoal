import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0

import "../items" as Items
import "../components" as Components

Components.PackageDataDetails {
    id: top

    onPackageDataChanged: {
        const data = [
           { "headerValue": Strings.nameOfProducer,      "inputValue": packageData.properties.name },
           { "headerValue": Strings.producerIdNumber,    "inputValue": packageData.properties.id },
           { "headerValue": Strings.village,             "inputValue": packageData.properties.village },
           { "headerValue": Strings.fieldCode,           "inputValue": packageData.properties.parcelCode },
           { "headerValue": Strings.harvestDate,         "inputValue": packageData.properties.breaking_date },
           { "headerValue": Strings.dateOfBreaking,      "inputValue": packageData.properties.departure_date }
       ]

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

            color: Style.headerBackgroundColor
            headerText: headerValue
            inputText: inputValue
        }
    }
}
