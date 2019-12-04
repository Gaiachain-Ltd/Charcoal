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

    relatedPackagesModel: relatedPackagesModel

    onPackageDataChanged: {
        if (typeof(top.packageData) === "undefined") {
            return
        }

        const data = [
           { "headerValue": Strings.organicCocoaTransporter,    "inputValue": emptyIfNotDefnied(packageData.properties[PackageDataProperties.Transporter]) },
           { "headerValue": Strings.destination,                "inputValue": emptyIfNotDefnied(packageData.properties[PackageDataProperties.Destination]) },
           { "headerValue": Strings.transportDate,              "inputValue": emptyIfNotDate(packageData.properties[PackageDataProperties.TransportDate]) }
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

    ListModel {
        id: detailsModel
    }

    ListModel {
        id: relatedPackagesModel
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
