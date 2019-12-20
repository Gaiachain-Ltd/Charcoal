import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.types 1.0

import "../components" as Components

Components.PackageDataDetails {
    id: top

    relatedPackagesModel: relatedPackagesModel

    onPackageDataChanged: {
        if (typeof(top.packageData) === "undefined") {
            return
        }

        var weightsMap = {}
        var weightsData = packageData.properties[PackageDataProperties.HarvestWeights]
        for (var i = 0; i < weightsData.length; ++i) {
            weightsMap[weightsData[i][PackageDataProperties.Pid]] = weightsData[i][PackageDataProperties.Weight]
        }

        var relatedPackages = packageData.relatedPackages

        relatedPackagesModel.clear()
        for (i = 0; i < relatedPackages.length; ++i) {
            var packageId = relatedPackages[i]
            var weight = weightsMap[packageId]
            relatedPackagesModel.append({ "headerValue": Strings.empty,
                                          "inputValue": packageId,
                                          "inputSuffixValue": Strings.kg.arg(minusIfNotDefinedOrZero(weight)) })
        }
    }

    ListModel {
        id: relatedPackagesModel
    }
}
