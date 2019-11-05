import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0

import "../components" as Components

Components.PackageDataDetails {
    id: top

    relatedPackagesModel: relatedPackagesModel

    onPackageDataChanged: {
        var relatedPackages = packageData.relatedPackages

        for (var i = 0; i < relatedPackages.length; ++i) {
            relatedPackagesModel.append({ "headerValue": Strings.empty,
                                          "inputValue": relatedPackages[i],
                                          "inputSuffixValue": Strings.kg.arg(packageData.properties.weights[relatedPackages[i]]) })
        }

    }

    ListModel {
        id: relatedPackagesModel
    }
}
